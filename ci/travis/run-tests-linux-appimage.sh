#!/usr/bin/env sh

mkdir bin
cd bin
wget "https://github.com/AppImage/AppImageKit/releases/download/10/appimagetool-x86_64.AppImage"
wget "https://github.com/AppImage/AppImageKit/releases/download/10/AppRun-x86_64"
chmod a+x appimagetool-x86_64.AppImage
chmod a+x AppRun-x86_64
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSEARCH_GDAL_PATH=${HOME}/.gdal -DSEARCH_PROJ_PATH=${HOME}/.proj -DSEARCH_GEOS_PATH=/usr -DMYSQL_MAIN_DIR=${HOME}/.mysql -DSEARCH_WXPDFDOCUMENT_PATH=${HOME}/.wxpdfdoc -DSEARCH_GEOS=1 -DSEARCH_GDAL=1 -DUSE_UNITTEST=1 -DCXXTEST_INCLUDE_DIR=/usr -DCXXTEST_PYTHON_TESTGEN_EXECUTABLE=/usr/bin/cxxtestgen -DUSE_APPIMAGE=1 ..
make -j$(nproc)
echo $(find package/linux-appimage/ -name "*linux64.AppImage")
cp $(find package/linux-appimage/ -name "*linux64.AppImage") .