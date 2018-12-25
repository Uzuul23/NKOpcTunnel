#define NkAppVersion "0.9"
#define NkAppPublisher "Henryk Anschütz / Berlin Germany"
#define NKAppName "NKOPCTunnel"
#define NKAppCopyright "Copyright (C) Henryk Anschütz"
#define MyAppURL "https://www.github.com/uzuul23"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{9C175A53-DDD8-42BF-AB29-D95ED146227A}

AppName={#NKAppName}
AppVersion={#NkAppVersion}
AppVerName={#NKAppName} {#NkAppVersion}
AppPublisher={#NkAppPublisher}
AppCopyright={#NKAppCopyright}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#NKAppName}
DefaultGroupName={#NKAppName}
OutputDir=.\
OutputBaseFilename={#NKAppName}_client_setup_v{#NkAppVersion}
Compression=lzma
SolidCompression=yes

; Cosmetic 
WizardImageFile=..\icon\VTOPCTunnel_banner_2.bmp
WizardSmallImageFile=..\icon\VTOPCTunnel.bmp
WizardImageStretch=yes

; windows version information
; 5.0.2195 Windows 2000 
; 5.1.2600 Windows XP or Windows XP 64-Bit Edition Version 2002 (Itanium) 
; 5.2.3790 Windows Server 2003 or Windows XP x64 Edition (AMD64/EM64T) or Windows XP 64-Bit Edition Version 2003 (Itanium) 
; 6.0.6000 Windows Vista 
; 6.1.7600 Windows 7 or Windows Server 2008 R2  
; 6.2.9200 Windows 8 or Windows Server 2012
; 6.3 Windows 8.1
; 10.0	Windows 10 Version

MinVersion=6.1

LicenseFile=..\LICENSE

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Types]
Name: "full"; Description: "Full installation";
Name: "client-only"; Description: "Client installation";
Name: "server-only"; Description: "Server installation";

[Components]
Name: "client"; Description: "Client Files"; Types: full client-only
Name: "server"; Description: "Server Files"; Types: full server-only

[Files]
; Source: "C:\Program Files (x86)\Inno Setup 5\Examples\MyProg.exe"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

; Client
Source: "..\bin\NkTnlClient.dll";  DestDir: "{app}\bin"; Flags: ignoreversion regserver; Components: client
Source: "..\bin\NkTnlClientCnf.exe";  DestDir: "{app}\bin"; Flags: ignoreversion; Components: client

; Server
Source: "..\bin\NkTnlSrv.exe";  DestDir: "{app}\bin"; Flags: ignoreversion; Components: server
Source: "..\bin\NkTnlServerCnf.exe";  DestDir: "{app}\bin"; Flags: ignoreversion; Components: server

[Registry]
; Root: HKLM; Subkey: "Software\WinPeSim"; Flags: uninsdeletekeyifempty
; Root: HKLM; Subkey: "Software\WinPeSim\NkOpcTunnel"; Flags: uninsdeletekey
; Root: HKLM; Subkey: "Software\WinPeSim\NkOpcTunnel\Settings"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"

[Icons]
; Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
; Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
; Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

