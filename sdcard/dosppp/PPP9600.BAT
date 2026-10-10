rem ZiModem PPP test for MURMULATOR COM1
rem COMTOOL:
rem   final X^ is a dummy token because ^ does not terminate a ^W string.
rem   ERRORLEVEL 0 = timeout/no match.
rem Edit WIFI_SSID and WIFI_PASSWORD before use.

set WIFI_SSID=your_ssid
set WIFI_PASSWORD=your_password

rem Start at ZiModem default baud: 1200 8N1
mode com1:1200,n,8,1

rem Check modem at 1200
comtool 1 AT^CR ^W20 OK ERROR X^
if errorlevel 2 goto error1200_reply
if errorlevel 1 goto at1200_ok
goto error1200_timeout

:at1200_ok

rem Connect WiFi explicitly.
rem ZiModem ATW can take more than 10 seconds, so allow 15 seconds.
comtool 1 ATW"%WIFI_SSID%,%WIFI_PASSWORD%"^CR ^W150 OK ERROR X^
if errorlevel 2 goto errorwifi_reply
if errorlevel 1 goto wifi_cmd_ok
goto errorwifi_timeout

:wifi_cmd_ok

rem Verify that ZiModem has an IPv4 address.
rem ATI2 outputs 0.0.0.0 when WiFi has no usable IP.
comtool 1 ATI2^CR ^W30 0.0.0.0 OK X^
if errorlevel 2 goto wifi_ip_ok
if errorlevel 1 goto errorwifiip
goto errorwifiip

:wifi_ip_ok

rem Switch ZiModem itself to 9600.
comtool 1 ATB9600^CR^

rem Switch DOS COM1 to the same baud.
mode com1:9600,n,8,1

rem Verify command channel at 9600
comtool 1 AT^CR ^W20 OK ERROR X^
if errorlevel 2 goto error9600_reply
if errorlevel 1 goto at9600_ok
goto error9600_timeout

:at9600_ok

rem Disable ZiModem command echo
comtool 1 ATE0^CR ^W20 OK ERROR X^
if errorlevel 2 goto error9600_reply
if errorlevel 1 goto ate0_ok
goto error9600_timeout

:ate0_ok

rem Enter PPP mode.
rem ZiModem may switch to binary PPP without leaving a final textual OK.
rem Explicit ERROR rejects the command; otherwise timeout means continue.
comtool 1 AT+PPP^CR ^W30 ERROR X Y^
if errorlevel 1 goto errorppp
goto ppp_start

:ppp_start

rem Start DOSPPP packet driver at 9600
lh epppdd base 0x3f8 irq 4 pktvec 0x60 9600 local asyncmap 0 debug kdebug 1 >1.log
goto end

:error1200_reply
echo ERROR: ZiModem returned ERROR to AT at 1200 baud.
goto end

:error1200_timeout
echo ERROR: timeout waiting for ZiModem AT reply at 1200 baud.
goto end

:errorwifi_reply
echo ERROR: ZiModem returned ERROR to WiFi ATW command.
goto end

:errorwifi_timeout
echo ERROR: timeout waiting for WiFi ATW command to finish.
goto end

:errorwifiip
echo ERROR: ZiModem has no usable WiFi IPv4 address.
goto end

:error9600_reply
echo ERROR: ZiModem returned ERROR on command channel at 9600 baud.
goto end

:error9600_timeout
echo ERROR: timeout on ZiModem command channel at 9600 baud.
goto end

:errorppp
echo ERROR: ZiModem rejected AT+PPP.
goto end

:end
