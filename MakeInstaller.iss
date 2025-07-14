[Setup]
AppName=Task Tracker
AppVersion=1.0
DefaultDirName={pf}\TaskTrackerApp
DefaultGroupName=Task Tracker
OutputDir=dist
OutputBaseFilename=Task Tracker Installer
Compression=lzma
SolidCompression=yes
WizardStyle=modern
SetupIconFile=D:\C++ Projects\Task Tracker\assets\icons\appinstaller_icon.ico

[Files]
Source: "D:\C++ Projects\Task Tracker\build\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\Task Tracker"; Filename: "{app}\TaskTracker.exe"
Name: "{group}\Uninstall Task Tracker"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\TaskTracker.exe"; Description: "{cm:LaunchProgram,Task Tracker}"; Flags: nowait postinstall skipifsilent
