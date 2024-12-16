[Setup]
AppName="SXFX Capture Client"
AppVersion=1.0
AppPublisher=2018wzh
DefaultDirName={commonpf}\capclient
DefaultGroupName="SXFX Capture Client"
Compression=lzma2
SolidCompression=yes
OutputDir=Output
OutputBaseFilename=sxfx_capclient_setup
[Files]
Source: "..\x64\Release\capclient.exe"; DestDir: "{app}\capclient.exe"

[Icons]
Name: "{group}\SXFX Capture Client"; Filename: "{app}\capclient.exe"

[Registry]
; ���ע�������������ʱ����Ӧ�ó���
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueName: "capclient"; ValueData: "{app}\capclient.exe"; Flags: uninsdeletevalue
