[Setup]
AppName=SXFX Capture Client
AppVersion=1.0
DefaultDirName={pf}\capclient
DefaultGroupName=SXFX Capture Client
UninstallDisplayIcon={app}\capclient.exe
Compression=lzma2
SolidCompression=yes
OutputDir=Bin

[Files]
Source: "InstallScript.iss"; DestDir: "{app}"

[Icons]
Name: "{group}\Inno_Setup_Project"; Filename: "{app}\Inno_Setup_Project.exe"