; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "ToolMap"
!define PRODUCT_VERSION '2.0'
!define PRODUCT_VERSION_LONG "2.0 Codename `Lugano`"
!define PRODUCT_PUBLISHER "CREALP"
!define PRODUCT_WEB_SITE "http://www.crealp.ch"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\ToolMap2.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\box-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\box-uninstall.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-uninstall.bmp"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
;!insertmacro MUI_PAGE_LICENSE "c:\dossier\vers\license\VotreLicenseLogicielle.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\ToolMap2.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "French"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "InstallToolMap.exe"
InstallDir "$PROGRAMFILES\ToolMap2"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "SectionPrincipale" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\..\..\..\bin\win32_VS2008\Debug\ToolMap2.exe"
  CreateDirectory "$SMPROGRAMS\ToolMap 2"
  CreateShortCut "$SMPROGRAMS\ToolMap 2\ToolMap 2.lnk" "$INSTDIR\ToolMap2.exe"
  CreateShortCut "$DESKTOP\ToolMap 2.lnk" "$INSTDIR\ToolMap2.exe"
  File "..\..\..\..\bin\win32_VS2008\Debug\gdal15.dll"
  File "..\..\..\..\bin\win32_VS2008\Debug\geos_c.dll"
  File "..\..\..\..\bin\win32_VS2008\Debug\libmysqld.dll"
  CreateDirectory "$INSTDIR\share"
  SetOutPath "$INSTDIR\share\charsets"
  SetOverwrite try
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\armscii8.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\ascii.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\cp1250.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\cp1251.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\cp1256.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\cp1257.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\cp850.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\cp852.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\cp866.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\dec8.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\geostd8.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\greek.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\hebrew.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\hp8.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\Index.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\keybcs2.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\koi8r.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\koi8u.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\languages.html"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\latin1.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\latin2.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\latin5.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\latin7.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\macce.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\macroman.xml"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\README"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\charsets\swe7.xml"
  SetOutPath "$INSTDIR\share\english"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\english\errmsg.sys"
  SetOutPath "$INSTDIR\share"
  File "..\..\..\..\bin\win32_VS2008\Debug\share\errmsg.txt"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\ToolMap 2\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\ToolMap 2\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\ToolMap2.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\ToolMap2.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) a �t� d�sinstall� avec succ�s de votre ordinateur."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "�tes-vous certains de vouloir d�sinstaller totalement $(^Name) et tous ses composants ?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\share\errmsg.txt"
  Delete "$INSTDIR\share\english\errmsg.sys"
  Delete "$INSTDIR\share\charsets\swe7.xml"
  Delete "$INSTDIR\share\charsets\README"
  Delete "$INSTDIR\share\charsets\macroman.xml"
  Delete "$INSTDIR\share\charsets\macce.xml"
  Delete "$INSTDIR\share\charsets\latin7.xml"
  Delete "$INSTDIR\share\charsets\latin5.xml"
  Delete "$INSTDIR\share\charsets\latin2.xml"
  Delete "$INSTDIR\share\charsets\latin1.xml"
  Delete "$INSTDIR\share\charsets\languages.html"
  Delete "$INSTDIR\share\charsets\koi8u.xml"
  Delete "$INSTDIR\share\charsets\koi8r.xml"
  Delete "$INSTDIR\share\charsets\keybcs2.xml"
  Delete "$INSTDIR\share\charsets\Index.xml"
  Delete "$INSTDIR\share\charsets\hp8.xml"
  Delete "$INSTDIR\share\charsets\hebrew.xml"
  Delete "$INSTDIR\share\charsets\greek.xml"
  Delete "$INSTDIR\share\charsets\geostd8.xml"
  Delete "$INSTDIR\share\charsets\dec8.xml"
  Delete "$INSTDIR\share\charsets\cp866.xml"
  Delete "$INSTDIR\share\charsets\cp852.xml"
  Delete "$INSTDIR\share\charsets\cp850.xml"
  Delete "$INSTDIR\share\charsets\cp1257.xml"
  Delete "$INSTDIR\share\charsets\cp1256.xml"
  Delete "$INSTDIR\share\charsets\cp1251.xml"
  Delete "$INSTDIR\share\charsets\cp1250.xml"
  Delete "$INSTDIR\share\charsets\ascii.xml"
  Delete "$INSTDIR\share\charsets\armscii8.xml"
  Delete "$INSTDIR\libmysqld.dll"
  Delete "$INSTDIR\geos_c.dll"
  Delete "$INSTDIR\gdal15.dll"
  Delete "$INSTDIR\ToolMap2.exe"

  Delete "$SMPROGRAMS\ToolMap 2\Uninstall.lnk"
  Delete "$SMPROGRAMS\ToolMap 2\Website.lnk"
  Delete "$DESKTOP\ToolMap 2.lnk"
  Delete "$SMPROGRAMS\ToolMap 2\ToolMap 2.lnk"

  RMDir "$SMPROGRAMS\ToolMap 2"
  RMDir "$INSTDIR\share\english"
  RMDir "$INSTDIR\share\charsets"
  RMDIR "$INSTDIR\share"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd