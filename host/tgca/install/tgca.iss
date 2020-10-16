#define MySrcDir "o:\work\"

#define VF
#define VS
#expr VF=FileOpen("..\version")
#expr VS=FileRead(VF)
#expr FileClose(VF)
#pragma message VS

[Setup]
#define MyAppName "tgca"
#define MyGrName "TGCA"
AppName={#MyAppName}_{#VS}
AppVersion={#VS}
;AppVerName={#MyAppName}{#VS}
AppPublisher=RC Module
AppPublisherURL=http://www.module.ru/
AppSupportURL=http://www.module.ru/
AppUpdatesURL=http://www.module.ru/
DefaultDirName={pf}\RC Module\{#MyGrName}_{#VS}
DefaultGroupName=Обработка 30
;OutputBaseFilename={#MyGrName}_setup
OutputBaseFileName={#MyGrName}_setup
Compression=lzma/normal

[Languages]
Name: russian; MessagesFile: compiler:Languages\Russian.isl

[Dirs]
;Name: "{app}\..\tgca"
;Name: "{app}\..\tgca_tests"
;Name: "{app}\..\default"

[Tasks]

[Files]
Source: {#MySrcDir}bin\*.*; DestDir: {app}\bin
Source: {#MySrcDir}bin\accessible\*.*; DestDir: {app}\bin\accessible
Source: {#MySrcDir}bin\bearer\*.*; DestDir: {app}\bin\bearer
Source: {#MySrcDir}bin\iconengines\*.*; DestDir: {app}\bin\iconengines
Source: {#MySrcDir}bin\imageformats\*.*; DestDir: {app}\bin\imageformats
Source: {#MySrcDir}bin\platforms\*.*; DestDir: {app}\bin\platforms
Source: {#MySrcDir}bin\printsupport\*.*; DestDir: {app}\bin\printsupport

Source: {#MySrcDir}projects\*.*; DestDir: {app}\bin 
Source: {#MySrcDir}tgca_tests\*.*; DestDir: {app}\tgca_tests
Source: {#MySrcDir}tgca\default\*.*; DestDir: {app}\default 
Source: {#MySrcDir}tgca\*.*; DestDir: {app}\tgca


[Icons]
Name: {group}\Обработка 30 {#MyAppName}. Версия {#VS}; Filename: {app}\bin\{#MyAppName}.exe; WorkingDir: {app}\bin

[Run]
