ECHO OFF
CLS

DEL /P *.7z *.zip
CLS

SET "sevzip="C:\Program Files\7-Zip\7z.exe""
SET "version=1.0.0.0"
SET "filelist=HLAEServer.dll HLAEServer.xml README.txt"

CD release_x86
%sevzip% a -t7z HLAEServer-%version%.7z %filelist%
%sevzip% a -tzip HLAEServer-%version%.zip %filelist%

MOVE HLAEServer-%version%.7z ..\
MOVE HLAEServer-%version%.zip ..\

ECHO.
ECHO Packaged.
PAUSE > nul