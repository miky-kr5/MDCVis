; MDCVis.nsi
;
; This script is based on the example2.nsi script included with the 
; NSIS distribution.
;
; It will install the MDCVis release into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "MDCVis"

; The file to write
OutFile "MDCVis_setup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\MDCVis

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\MDCVis" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "MDCVis (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "LICENSE"
  File "Credits.md"
  File "bin\MDCVis.exe"
  File "lib\Windows\Irrlicht.dll"
  File "data\mdc.zip"
  File /r "data\exhibits"
  File /r "data\font"
  File /r "data\gfx"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\MDCVis "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MDCVis" "DisplayName" "MDCVis"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MDCVis" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MDCVis" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MDCVis" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\MDCVis"
  CreateShortCut "$SMPROGRAMS\MDCVis\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\MDCVis\Museo de Ciencias.lnk" "$INSTDIR\MDCVis.exe" "" "$INSTDIR\MDCVis.exe" 0
  CreateShortCut "$SMPROGRAMS\MDCVis\LICENSE.lnk" "$INSTDIR\LICENSE" "" "$INSTDIR\LICENSE" 0
  CreateShortCut "$SMPROGRAMS\MDCVis\CREDITS.lnk" "$INSTDIR\CREDITS.md" "" "$INSTDIR\CREDITS.md" 0
  
SectionEnd

;--------------------------------

; Optional section (can be disabled by the user)
Section "Desktop Shortcuts"

  CreateShortCut "$DESKTOP\Museo de Ciencias.lnk" "$INSTDIR\MDCVis.exe" "" "$INSTDIR\MDCVis.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MDCVis"
  DeleteRegKey HKLM SOFTWARE\MDCVis

  ; Remove files and uninstaller
  Delete "$INSTDIR\MDCVis.exe"
  Delete "$INSTDIR\Irrlicht.dll"
  Delete "$INSTDIR\mdc.zip"
  Delete "$INSTDIR\LICENSE"
  Delete "$INSTDIR\Credits.md"
  Delete "$INSTDIR\exhibits\*.*"
  Delete "$INSTDIR\font\*.*"
  Delete "$INSTDIR\gfx\*.*"
  Delete "$INSTDIR\uninstall.exe"
  RMDir "$INSTDIR\exhibits"
  RMDir "$INSTDIR\font"
  RMDir "$INSTDIR\gfx"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\MDCVis\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\MDCVis"
  RMDir "$INSTDIR"
  
  ; Remove desktop shortcut
  Delete "$DESKTOP\Museo de Ciencias.lnk"

SectionEnd
