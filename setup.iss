; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Cars Scraper"
#define MyAppVersion "1.0"
#define MyAppExeName "cars_scraper.exe"
#define QtDir GetEnv("qtdir")

[Setup]
AppId={{8551FD1F-143C-4409-AFF0-D2A0270FB089}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir=..\..\bin\cars_scraper\
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\..\bin\cars_scraper\cars_scraper\Release\cars_scraper.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\bin\cars_scraper\cars_scraper\Release\settings.ini"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtDir}bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtDir}bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtDir}bin\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtDir}bin\QtWebKit4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtDir}bin\QtXml4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtDir}bin\QtSvg4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QtDir}plugins\*"; DestDir: "{app}\plugins"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "libs\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent