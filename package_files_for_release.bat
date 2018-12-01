ECHO OFF
CLS

DEL /P *.7z *.zip
CLS

SET "sevzip="C:\Program Files\7-Zip\7z.exe""
SET "version=1.0.1.0"
SET "filelist=HLAEServer.dll Release_x86 Release_x64 README.txt"

CD release_x86
%sevzip% a -t7z HLAEServer-%version%.7z %filelist%
%sevzip% a -tzip HLAEServer-%version%.zip %filelist%

MOVE HLAEServer-%version%.7z ..\
MOVE HLAEServer-%version%.zip ..\

ECHO.
ECHO Packaged.
PAUSE > nul