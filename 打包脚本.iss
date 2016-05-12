[Setup]
AppId={{C664F1FE-DF81-4B2B-BB11-EF4DEA854605}
AppName=个人记帐本
AppVerName=个人记帐本 1.12
AppPublisher=银碟软件工作室QQ5069099
AppPublisherURL=http://5069099.taobao.com/
AppSupportURL=http://5069099.taobao.com/
AppUpdatesURL=http://5069099.taobao.com/
DefaultDirName=E:\Sdup
DefaultGroupName=Sdup
DisableProgramGroupPage=yes
OutputDir=.\
OutputBaseFilename=setup112
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinese"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce

[Files]
Source: ".\Release\Sdup.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\Release\mfc100.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: ".\Release\msvcr100.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: ".\Release\Sdup.dll"; DestDir: "{app}";DestName:"Bak.dll"; Flags: ignoreversion
Source: ".\MSADO15.dll"; DestDir: "{app}"; Flags: ignoreversion


[Icons]
Name: "{group}\个人记帐本"; Filename: "{app}\Sdup.exe"
Name: "{commondesktop}\个人记帐本"; Filename: "{app}\Sdup.exe"; Tasks: desktopicon
Name: "{group}\{cm:ProgramOnTheWeb,个人记帐本官网}"; Filename: "http://5069099.taobao.com/"

[Run]
Filename: "{app}\Sdup.exe"; Description: "{cm:LaunchProgram,Sdup}"; Flags: nowait postinstall skipifsilent

