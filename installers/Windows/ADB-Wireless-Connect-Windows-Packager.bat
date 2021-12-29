@echo off
echo ADB Wireless Connect Windows Packager 

echo Setting paths...
setlocal
SET PATH=%PATH%;C:\Qt\6.2.2\mingw_64\bin
SET PATH=%PATH%;C:\Qt\Tools\QtInstallerFramework\4.2\bin
SET PATH=%PATH%;C:\Qt\Tools\mingw900_64\bin

echo Deleting old installer...
del ADB-Wireless-Connect-Windows-Installer.exe
del .\packages\com.adbwirelessconnect\data\adb-wireless-connect.7z

echo Creating Windows executable using windeployqt... 
windeployqt "..\..\builds\Windows\release\adb-wireless-connect\ADBWirelessConnect.exe"


echo Packaging application... 
archivegen .\packages\com.adbwirelessconnect\data\adb-wireless-connect.7z ..\..\builds\Windows\release\adb-wireless-connect

echo Creating installer...
binarycreator.exe --offline-only  -c config/config.xml -p packages ADB-Wireless-Connect-Windows-Installer.exe

echo Deleting temporary packages...
del .\packages\com.adbwirelessconnect\data\adb-wireless-connect.7z

echo Packaging finished.
echo The installer is ADB-Wireless-Connect-Windows-Installer.exe
pause
