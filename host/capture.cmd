@echo off
chcp 65001 >nul
powershell.exe -NoLogo -NoProfile -ExecutionPolicy Bypass -File "%~dp0capture.ps1" -WaitSeconds 120 -CaptureSeconds 15
set "capture_exit=%ERRORLEVEL%"
pause
exit /b %capture_exit%
