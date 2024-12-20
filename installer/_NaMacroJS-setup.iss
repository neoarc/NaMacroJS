; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#include "../version.h"

#define MyAppName         PRODUCT_NAME
#define MyAppVersion      PRODUCT_VERSION_STR
#define MyAppPublisher    COMPANY_NAME
#define MyAppURL          PRODUCT_URL
#define MyAppExeName     "NaMacroRecorder.exe"
#define MyAppProductName  PRODUCT_NAME

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{09EAEEA3-36DF-4F8D-BE23-E9935AD1B017}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
OutputBaseFilename=NaMacroJS-setup
SetupIconFile=..\NaMacro.ico
Compression=lzma2
SolidCompression=yes
OutputDir=output
VersionInfoCompany={#MyAppPublisher}
VersionInfoProductName={#MyAppProductName}
VersionInfoVersion={#MyAppVersion}
VersionInfoCopyright={#MyAppURL}
ChangesAssociations=true

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "bin_src\*.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin_src\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin_src\scripts\*"; DestDir: "{app}\scripts"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
; njs file association
;   ref: http://www.jrsoftware.org/isfaq.php#assoc
Root: HKCR; Subkey: ".njs"; ValueType: string; ValueName: ""; ValueData: "NaMacroJS"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "NaMacroJS"; ValueType: string; ValueName: ""; ValueData: "NaMacroJS script file"; Flags: uninsdeletekey
Root: HKCR; Subkey: "NaMacroJS\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\NaMacro.exe,1"
Root: HKCR; Subkey: "NaMacroJS\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\NaMacro.EXE"" ""%1"""

[Icons]
;Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
