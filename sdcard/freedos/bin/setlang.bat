@ECHO OFF
REM Copyright 2022 Jerome Shidel and Wilhem Spiegl
REM GNU General Public License, version 2 or later

REM This batch will be updated in the future to provide language setting and
REM switching for the installed system. Including, making changes to the
REM language specific version of FreeCOM.

if /I "%1" == "HELP" goto ShowHelp
if /I "%1" == "/?" goto ShowHelp
if /I "%1" == "SIMPLE" goto RequestSimple
goto MaybeNotSimple

:ShowHelp
echo help not present at present.
goto End

:RequestSimple
shift

:SimpleMode
vfont /d
SET V8F=error
if /I "%1" == "EN" set V8F=
if /I "%1" == "ES" set V8F=
if /I "%1" == "FR" set V8F=
if /I "%1" == "DE" set V8F=CP858.V8F
if /I "%1" == "TR" set V8F=CP857.V8F
if /I "%1" == "EO" set V8F=
if /I "%1" == "NL" set V8F=CP858.V8F
if /I "%1" == "RU" set V8F=CP866.V8F
if /I "%1" == "TR2" set V8F=CP857.V8F
if /I "%1" == "SV" set V8F=CP850.V8F
if /I "%V8F%" == "ERROR" goto UnknownLANG

if "%V8F%" == "" goto FontIsSet
if not exist %DOSDIR%\BIN\%V8F% goto NotBINDir
vfont %DOSDIR%\BIN\%V8F%
goto FontIsSet
:NotBINDir
if not exist %DOSDIR%\SETUP\%V8F% goto FontNotFound
vfont %DOSDIR%\SETUP\%V8F%

:FontIsSet
set LANG=%1
if /I "%1" == "TR2" set LANG=TR
if "%LANG%" == "" set LANG=EN
goto End

:MaybeNotSimple
if not exist %DOSDIR%\BIN\DISPLAY.EXE goto SimpleMode
if not exist %DOSDIR%\BIN\COUNTRY.SYS goto SimpleMode

SET CPIPATH=%DOSDIR%\BIN
if not exist %CPIPATH%\EGA.CPX SET CPIPATH=%DOSDIR%\CPI
if not exist %CPIPATH%\EGA.CPX goto SimpleMode

if /I "%1" == "EN" goto LANG_EN
if /I "%1" == "ES" goto LANG_ES
if /I "%1" == "FR" goto LANG_FR
if /I "%1" == "DE" goto LANG_DE
if /I "%1" == "TR" goto LANG_TR
if /I "%1" == "EO" goto LANG_EO
if /I "%1" == "NL" goto LANG_NL
if /I "%1" == "RU" goto LANG_RU
if /I "%1" == "TR2" goto LANG_TR2
if /I "%1" == "SV" goto LANG_SV
goto UnknownLANG

REM EN, English
:LANG_EN
REM COUNTRY=001,437,%DOSDIR%\BIN\COUNTRY.SYS
SET LANG=EN
rem lh %DOSDIR%\BIN\mkeyb us
GOTO END_LANG

REM ES, Spanish
:LANG_ES
REM COUNTRY=034,850,%DOSDIR%\BIN\COUNTRY.SYS
SET LANG=ES
lh %DOSDIR%\BIN\DISPLAY.EXE CON=(EGA,850,1)
%DOSDIR%\BIN\MODE CON CP PREP=((850) %CPIPATH%\EGA.CPX)
%DOSDIR%\BIN\MODE CON CP SEL=850
%DOSDIR%\BIN\MODE CON CP REFRESH
%DOSDIR%\BIN\MODE CON CP /STATUS
lh %DOSDIR%\BIN\mkeyb sp
GOTO END_LANG

REM FR, French
:LANG_FR
REM COUNTRY=033,850,%DOSDIR%\BIN\COUNTRY.SYS
SET LANG=FR
lh %DOSDIR%\BIN\DISPLAY.EXE CON=(EGA,850,1)
%DOSDIR%\BIN\MODE CON CP PREP=((850) %CPIPATH%\EGA.CPX)
%DOSDIR%\BIN\MODE CON CP SEL=850
%DOSDIR%\BIN\MODE CON CP REFRESH
%DOSDIR%\BIN\MODE CON CP /STATUS
lh %DOSDIR%\BIN\mkeyb fr
GOTO END_LANG

REM DE, German
:LANG_DE
REM COUNTRY=049,850,%DOSDIR%\BIN\COUNTRY.SYS
SET LANG=DE
lh %DOSDIR%\BIN\DISPLAY.EXE CON=(EGA,850,1)
%DOSDIR%\BIN\MODE CON CP PREP=((850) %CPIPATH%\EGA.CPX)
%DOSDIR%\BIN\MODE CON CP SEL=850
%DOSDIR%\BIN\MODE CON CP REFRESH
%DOSDIR%\BIN\MODE CON CP /STATUS
lh %DOSDIR%\BIN\mkeyb gr
GOTO END_LANG

REM TR, Turkish
:LANG_TR
REM COUNTRY=090,857,%DOSDIR%\BIN\COUNTRY.SYS
SET LANG=TR
lh %DOSDIR%\BIN\DISPLAY.EXE CON=(EGA,857,1)
%DOSDIR%\BIN\MODE CON CP PREP=((857) %CPIPATH%\EGA.CPX)
%DOSDIR%\BIN\MODE CON CP SEL=857
%DOSDIR%\BIN\MODE CON CP REFRESH
%DOSDIR%\BIN\MODE CON CP /STATUS
lh %DOSDIR%\BIN\keyb tr,857,%DOSDIR%\BIN\keybrd2.sys
REM lh %DOSDIR%\BIN\mkeyb fr
GOTO END_LANG

REM EO, Esperanto
:LANG_EO
REM COUNTRY=001,850,%DOSDIR%\BIN\COUNTRY.SYS
SET LANG=EO
lh %DOSDIR%\BIN\mkeyb us
REM lh %DOSDIR%\BIN\keyb tr.key
GOTO END_LANG

REM NL, Dutch Netherlands
:LANG_NL
REM COUNTRY=031,850,%DOSDIR%\BIN\COUNTRY.SYS
SET LANG=NL
lh %DOSDIR%\BIN\DISPLAY.EXE CON=(EGA,850,1)
%DOSDIR%\BIN\MODE CON CP PREP=((850) %CPIPATH%\EGA.CPX)
%DOSDIR%\BIN\MODE CON CP SEL=850
%DOSDIR%\BIN\MODE CON CP REFRESH
%DOSDIR%\BIN\MODE CON CP /STATUS
lh %DOSDIR%\BIN\mkeyb nl
GOTO END_LANG

REM RU, Russian
:LANG_RU
REM COUNTRY=007,866,%DOSDIR%\BIN\COUNTRY.SYS
SET LANG=RU
lh %DOSDIR%\BIN\DISPLAY CON=(EGA,,1)
%DOSDIR%\BIN\MODE CON CP PREP=((866) %CPIPATH%\EGA3.CPX)
%DOSDIR%\BIN\MODE CON CP SEL=866
%DOSDIR%\BIN\MODE CON CP REFRESH
%DOSDIR%\BIN\MODE CON CP /STATUS
lh %DOSDIR%\BIN\KEYB RU,,keybrd3.sys
GOTO END_LANG

REM TR2, Turkish 2
:LANG_TR2
REM COUNTRY=090,857,%DOSDIR%\BIN\COUNTRY.SYS
SET LANG=TR
lh %DOSDIR%\BIN\DISPLAY.EXE CON=(EGA,3846,1)
%DOSDIR%\BIN\MODE CON CP PREP=((3846) %CPIPATH%\EGA18.CPX)
%DOSDIR%\BIN\MODE CON CP SEL=3846
%DOSDIR%\BIN\MODE CON CP REFRESH
%DOSDIR%\BIN\MODE CON CP /STATUS
lh %DOSDIR%\BIN\keyb tr,3846,%DOSDIR%\BIN\keybrd2.sys
GOTO END_LANG

REM SV, Swedish
:LANG_SV
REM COUNTRY=041,850,%DOSDIR%\BIN\COUNTRY.SYS
SET LANG=SV
lh %DOSDIR%\BIN\DISPLAY.EXE CON=(EGA,850,2)
%DOSDIR%\BIN\MODE CON CP PREP=((850) %CPIPATH%\EGA.CPX)
%DOSDIR%\BIN\MODE CON CP SEL=850
%DOSDIR%\BIN\MODE CON CP REFRESH
%DOSDIR%\BIN\MODE CON CP /STATUS
lh %DOSDIR%\BIN\mkeyb sv
GOTO END_LANG

:END_LANG

REM Possibly make change Contry settings in FDCONFIG.
REM Possiblt swap out the current version of FreeCOM.
REM Suggest reboot.
goto End

:FontNotFound
echo Unable to locate %V8F% font for %1 language.
goto ErrorEnd

:UnknownLANG
echo %1 is not a language known by this utility.

:ErrorEnd
verrlvl 1

:End
set V8F=
set CPIPATH=