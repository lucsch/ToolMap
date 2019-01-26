# Options
$TMP_DIR="C:\projects\tmp"
$LIB_DIR="C:\projects\libs"
$CMAKE_DIR="C:\projects\cmake"
$WIX_DIR="C:\projects\wix"
$CXXTEST_DIR="C:\projects\cxxtest"
$PATCH_DIR="C:\projects\toolmap\ci\appveyor\patches"
$MSC_VER=1915
$ON_APPVEYOR=$true
$WITH_DEBUG_LIBS=$false
$MYSQL_BUILD_TYPE="RelWithDebInfo"

# Force rebuilding some libraries
$REBUILD_WX=$false
$REBUILD_WXPDF=$false
$REBUILD_PROJ=$false
$REBUILD_GEOS=$false
$REBUILD_GDAL=$false
$REBUILD_MYSQL=$false
$REBUILD_CURL=$false

# Setup VS environment
# https://stackoverflow.com/questions/2124753/how-can-i-use-powershell-with-the-visual-studio-command-prompt
pushd 'C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build'    
cmd /c "vcvars64.bat&set" |
foreach {
  if ($_ -match "=") {
    $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
  }
}
popd
Write-Host "`nVisual Studio 2017 Command Prompt variables set." -ForegroundColor Yellow

# All external dependencies are installed in the defined directory
if(-not (Test-Path -Path $LIB_DIR)) {
  mkdir $LIB_DIR > $null
}
if(-not (Test-Path -Path $TMP_DIR)) {
  mkdir $TMP_DIR > $null
}

# Install a recent CMake
Write-Host "`nInstalling CMake" -ForegroundColor Yellow
cd $TMP_DIR
$CMAKE_URL="https://cmake.org/files/v3.9/cmake-3.9.4-win64-x64.zip"
if ($ON_APPVEYOR) {
  appveyor DownloadFile $CMAKE_URL -FileName cmake.zip > $null
} else {
  Invoke-WebRequest -Uri $CMAKE_URL -OutFile cmake.zip
}
7z x cmake.zip -o"$TMP_DIR" > $null
move "$TMP_DIR\cmake-*" "$CMAKE_DIR"
$path = $env:Path
$path = ($path.Split(';') | Where-Object { $_ -ne 'C:\Program Files (x86)\CMake\bin' }) -join ';'
$path = ($path.Split(';') | Where-Object { $_ -ne 'C:\Tools\NuGet' }) -join ';'
$env:Path = $path
$env:Path += ";$CMAKE_DIR\bin"
cmake --version

# Install WIX
Write-Host "`nInstalling WIX" -ForegroundColor Yellow
cd $TMP_DIR
$WIX_URL="https://github.com/wixtoolset/wix3/releases/download/wix3111rtm/wix311-binaries.zip"
if ($ON_APPVEYOR) {
  appveyor DownloadFile $WIX_URL -FileName wix.zip > $null
} else {
  Invoke-WebRequest -Uri $WIX_URL -OutFile wix.zip
}
7z x wix.zip -o"$TMP_DIR" > $null
move "$TMP_DIR\wix311-binaries\*" "$WIX_DIR"
$env:Path += ";$WIX_DIR\bin"

# Install cxxtest
Write-Host "`nInstalling cxxtest" -ForegroundColor Yellow
cd $TMP_DIR
$CXXTEST_URL="https://github.com/CxxTest/cxxtest/archive/4.3.zip"
if ($ON_APPVEYOR) {
  appveyor DownloadFile $CXXTEST_URL -FileName cxxtest.zip > $null
} else {
  Invoke-WebRequest -Uri $CXXTEST_URL -OutFile cxxtest.zip
}
7z x cxxtest.zip -o"$TMP_DIR" > $null
move "$TMP_DIR\cxxtest-*" "$CXXTEST_DIR"
$env:Path += ";$CXXTEST_DIR"

# List dirs already present from cache
Write-Host "`nContent of the cache:" -ForegroundColor Yellow
Get-ChildItem "$LIB_DIR"
  
# Install wxWidgets
if(-not (Test-Path -Path "$LIB_DIR\wxwidgets") -Or $REBUILD_WX) {
  Write-Host "`nBuilding wxWidgets" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\wxwidgets") {
    Remove-Item "$LIB_DIR\wxwidgets" -Force -Recurse
  }
  mkdir "$LIB_DIR\wxwidgets" > $null
  $WX_URL="https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $WX_URL -FileName wxwidgets.zip > $null
  } else {
    Invoke-WebRequest -Uri $WX_URL -OutFile wxwidgets.zip
  }
  7z x wxwidgets.zip -o"$TMP_DIR\wxwidgets" > $null
  cd "$TMP_DIR\wxwidgets\build\msw"
  nmake -f makefile.vc BUILD=release UNICODE=1 MONOLITHIC=1 > $null
  nmake -f makefile.vc BUILD=debug UNICODE=1 MONOLITHIC=1 > $null
  move "$TMP_DIR\wxwidgets\include" "$LIB_DIR\wxwidgets\include"
  copy "$TMP_DIR\wxwidgets\lib\vc_lib\mswu\wx\setup.h" "$LIB_DIR\wxwidgets\include\wx\setup.h"
  move "$LIB_DIR\wxwidgets\include\wx\msw\rcdefs.h" "$LIB_DIR\wxwidgets\include\wx\msw\rcdefs.h_old"
  copy "$TMP_DIR\wxwidgets\lib\vc_lib\mswu\wx\msw\rcdefs.h" "$LIB_DIR\wxwidgets\include\wx\msw\rcdefs.h"
  move "$TMP_DIR\wxwidgets\lib" "$LIB_DIR\wxwidgets\lib"
} else {
  Write-Host "`nwxWidgets already in cache" -ForegroundColor Yellow
}
$env:WXWIN = "$LIB_DIR\wxwidgets"
# List files
Get-ChildItem "$LIB_DIR/wxwidgets"

# Install wxPDFDocument
if(-not (Test-Path -Path "$LIB_DIR\wxpdfdoc") -Or $REBUILD_WXPDF) {
  Write-Host "`nBuilding wxPDFDocument" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\wxpdfdoc") {
    Remove-Item "$LIB_DIR\wxpdfdoc" -Force -Recurse
  }
  mkdir "$LIB_DIR\wxpdfdoc" > $null
  $WXPDF_URL="https://github.com/utelle/wxpdfdoc/releases/download/v0.9.5/wxpdfdoc-0.9.5.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $WXPDF_URL -FileName wxpdfdoc.zip > $null
  } else {
    Invoke-WebRequest -Uri $WXPDF_URL -OutFile wxpdfdoc.zip
  }
  7z x wxpdfdoc.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\wxpdfdoc-*" "$TMP_DIR\wxpdfdoc"
  cd "$TMP_DIR\wxpdfdoc\build"
  nmake -f makefile.vc WX_DIR="$LIB_DIR\wxwidgets" WX_VERSION=31 WX_MONOLITHIC=1 WX_DEBUG=0 > $null
  nmake -f makefile.vc WX_DIR="$LIB_DIR\wxwidgets" WX_VERSION=31 WX_MONOLITHIC=1 WX_DEBUG=1 > $null
  move "$TMP_DIR\wxpdfdoc\include" "$LIB_DIR\wxpdfdoc\include"
  move "$TMP_DIR\wxpdfdoc\lib" "$LIB_DIR\wxpdfdoc\lib"
} else {
  Write-Host "`nwxPDFDocument already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/wxpdfdoc"

# Install curl
if(-not (Test-Path -Path "$LIB_DIR\curl") -Or $REBUILD_CURL) {
  Write-Host "`nBuilding curl" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\curl") {
    Remove-Item "$LIB_DIR\curl" -Force -Recurse
  }
  mkdir "$LIB_DIR\curl" > $null
  $CURL_URL="https://github.com/curl/curl/archive/curl-7_54_1.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $CURL_URL -FileName curl.zip > $null
  } else {
    Invoke-WebRequest -Uri $CURL_URL -OutFile curl.zip
  }
  7z x curl.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\curl-*" "$TMP_DIR\curl"
  cd "$TMP_DIR\curl\winbuild"
  nmake -f Makefile.vc mode=dll VC=14 DEBUG=NO MACHINE=x64 > $null
  move "$TMP_DIR\curl\builds\libcurl-vc14-x64-release-dll-ipv6-sspi-winssl\bin" "$LIB_DIR\curl\bin"
  move "$TMP_DIR\curl\builds\libcurl-vc14-x64-release-dll-ipv6-sspi-winssl\include" "$LIB_DIR\curl\include"
  move "$TMP_DIR\curl\builds\libcurl-vc14-x64-release-dll-ipv6-sspi-winssl\lib" "$LIB_DIR\curl\lib"
} else {
  Write-Host "`curl already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/curl"

# Install Proj
if(-not (Test-Path -Path "$LIB_DIR\proj") -Or $REBUILD_PROJ) {
  Write-Host "`nBuilding Proj" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\proj") {
    Remove-Item "$LIB_DIR\proj" -Force -Recurse
  }
  mkdir "$LIB_DIR\proj" > $null
  $PROJ_URL="https://github.com/OSGeo/proj.4/archive/4.9.3.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $PROJ_URL -FileName proj.zip > $null
  } else {
    Invoke-WebRequest -Uri $PROJ_URL -OutFile proj.zip
  }
  7z x proj.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\proj.4-*" "$TMP_DIR\proj"
  cd "$TMP_DIR\proj"
  nmake -f makefile.vc INSTDIR="$LIB_DIR\proj" > $null
  nmake -f makefile.vc INSTDIR="$LIB_DIR\proj" install-all > $null
} else {
  Write-Host "`Proj already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/proj"

# Install Geos
if(-not (Test-Path -Path "$LIB_DIR\geos") -Or $REBUILD_GEOS) {
  Write-Host "`nBuilding Geos" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\geos") {
    Remove-Item "$LIB_DIR\geos" -Force -Recurse
  }
  mkdir "$LIB_DIR\geos" > $null
  $GEOS_URL="http://download.osgeo.org/geos/geos-3.6.1.tar.bz2"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $GEOS_URL -FileName geos.tar.bz2 > $null
  } else {
    Invoke-WebRequest -Uri $GEOS_URL -OutFile geos.tar.bz2
  }
  7z x geos.tar.bz2 -o"$TMP_DIR" > $null
  7z x geos.tar -o"$TMP_DIR" > $null
  move "$TMP_DIR\geos-*" "$TMP_DIR\geos"
  rm "$TMP_DIR\geos\nmake.opt"
  copy "$PATCH_DIR\geos-3.6.1-nmake.opt" "$TMP_DIR\geos\nmake.opt"
  cd "$TMP_DIR\geos"
  nmake -f makefile.vc WIN64=YES > $null
  if ($WITH_DEBUG_LIBS) {
    nmake -f makefile.vc WIN64=YES BUILD_DEBUG=YES > $null
  }
  move "$TMP_DIR\geos\src" "$LIB_DIR\geos\src"
  move "$TMP_DIR\geos\include" "$LIB_DIR\geos\include"
  move "$TMP_DIR\geos\capi" "$LIB_DIR\geos\capi"
} else {
  Write-Host "`Geos already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/geos"

# Install Gdal
if(-not (Test-Path -Path "$LIB_DIR\gdal") -Or $REBUILD_GDAL) {
  Write-Host "`nBuilding Gdal" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\gdal") {
    Remove-Item "$LIB_DIR\gdal" -Force -Recurse
  }
  mkdir "$LIB_DIR\gdal" > $null
  $GDAL_URL="http://download.osgeo.org/gdal/2.4.0/gdal240.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $GDAL_URL -FileName gdal.zip > $null
  } else {
    Invoke-WebRequest -Uri $GDAL_URL -OutFile gdal.zip
  }
  7z x gdal.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\gdal-*" "$TMP_DIR\gdal"
  cd "$TMP_DIR\gdal"
  $LIB_DIR_REV=$LIB_DIR -replace '\\','/'
  nmake -f makefile.vc MSVC_VER=$MSC_VER WIN64=1 GDAL_HOME="$LIB_DIR\gdal" GEOS_DIR="$LIB_DIR_REV/geos" GEOS_CFLAGS="-I$LIB_DIR_REV/geos/capi -I$LIB_DIR_REV/geos/include -DHAVE_GEOS" GEOS_LIB="$LIB_DIR_REV/geos/src/geos_c_i.lib" CURL_DIR="$LIB_DIR\curl" CURL_INC="-I$LIB_DIR_REV/curl/include" CURL_LIB="$LIB_DIR_REV/curl/lib/libcurl.lib wsock32.lib wldap32.lib winmm.lib" CURL_CFLAGS=-DCURL_STATICLIB > $null
  nmake -f makefile.vc MSVC_VER=$MSC_VER WIN64=1 GDAL_HOME="$LIB_DIR\gdal" GEOS_DIR="$LIB_DIR_REV/geos" GEOS_CFLAGS="-I$LIB_DIR_REV/geos/capi -I$LIB_DIR_REV/geos/include -DHAVE_GEOS" GEOS_LIB="$LIB_DIR_REV/geos/src/geos_c_i.lib" CURL_DIR="$LIB_DIR\curl" CURL_INC="-I$LIB_DIR_REV/curl/include" CURL_LIB="$LIB_DIR_REV/curl/lib/libcurl.lib wsock32.lib wldap32.lib winmm.lib" CURL_CFLAGS=-DCURL_STATICLIB install > $null
  nmake -f makefile.vc MSVC_VER=$MSC_VER WIN64=1 GDAL_HOME="$LIB_DIR\gdal" GEOS_DIR="$LIB_DIR_REV/geos" GEOS_CFLAGS="-I$LIB_DIR_REV/geos/capi -I$LIB_DIR_REV/geos/include -DHAVE_GEOS" GEOS_LIB="$LIB_DIR_REV/geos/src/geos_c_i.lib" CURL_DIR="$LIB_DIR\curl" CURL_INC="-I$LIB_DIR_REV/curl/include" CURL_LIB="$LIB_DIR_REV/curl/lib/libcurl.lib wsock32.lib wldap32.lib winmm.lib" CURL_CFLAGS=-DCURL_STATICLIB devinstall > $null
} else {
  Write-Host "`Gdal already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/gdal"

# Install Mysql
if(-not (Test-Path -Path "$LIB_DIR\mysql") -Or $REBUILD_MYSQL) {
  Write-Host "`nBuilding Mysql" -ForegroundColor Yellow
  cd $TMP_DIR
  if(Test-Path -Path "$LIB_DIR\mysql") {
    Remove-Item "$LIB_DIR\mysql" -Force -Recurse
  }
  mkdir "$LIB_DIR\mysql" > $null
  $MYSQL_URL="https://dev.mysql.com/get/Downloads/MySQL-5.6/mysql-5.6.36.zip"
  if ($ON_APPVEYOR) {
    appveyor DownloadFile $MYSQL_URL -FileName mysql.zip > $null
  } else {
    Invoke-WebRequest -Uri $MYSQL_URL -OutFile mysql.zip
  }
  7z x mysql.zip -o"$TMP_DIR" > $null
  move "$TMP_DIR\mysql-*" "$TMP_DIR\mysql"
  cd "$TMP_DIR\mysql"
  rm "$TMP_DIR\mysql\sql\sql_table.cc"
  copy "$PATCH_DIR\mysql-5.6.36-sql_table.cc" "$TMP_DIR\mysql\sql\sql_table.cc"
  cmake . -G"Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=$MYSQL_BUILD_TYPE -DCMAKE_INSTALL_PREFIX="$LIB_DIR\mysql" -DWITH_UNIT_TESTS:BOOL=OFF -DFEATURE_SET:STRING=small > $null
  if ($ON_APPVEYOR) {
    cmake --build . --config $MYSQL_BUILD_TYPE --target sql > $null
    cmake --build . --config $MYSQL_BUILD_TYPE --target libmysqld > $null
    mkdir "$LIB_DIR\mysql\lib" > $null
    copy "$TMP_DIR\mysql\libmysqld\$MYSQL_BUILD_TYPE\libmysqld.lib" "$LIB_DIR\mysql\lib\libmysqld.lib"
    copy "$TMP_DIR\mysql\libmysqld\$MYSQL_BUILD_TYPE\libmysqld.dll" "$LIB_DIR\mysql\lib\libmysqld.dll"
    Copy-Item "$TMP_DIR\mysql\include" -Destination "$LIB_DIR\mysql\include" -Recurse
    Copy-Item "$TMP_DIR\mysql\sql\share" -Destination "$LIB_DIR\mysql\share" -Recurse
  } else {
    cmake --build . --config $MYSQL_BUILD_TYPE
    cmake --build . --config $MYSQL_BUILD_TYPE --target INSTALL
  }
} else {
  Write-Host "`Mysql already in cache" -ForegroundColor Yellow
}
# List files
Get-ChildItem "$LIB_DIR/mysql"
