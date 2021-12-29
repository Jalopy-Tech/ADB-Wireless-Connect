#!/bin/bash
echo "ADB Wireless Connect macOS Packager" 

echo "Deleting old installer..."
rm ./ADB-Wireless-Connect-macOS-Installer
rm ./packages/com.adbwirelessconnect/data/adb-wireless-connect.7z



echo "Packaging application..." 
~/Qt/Tools/QtInstallerFramework/4.2/bin/archivegen ./packages/com.adbwirelessconnect/data/adb-wireless-connect.7z ../../builds/macOS/release/adb-wireless-connect

echo "Creating installer..."
~/Qt/Tools/QtInstallerFramework/4.2/bin/binarycreator --offline-only  -c config/config.xml -p packages ADB-Wireless-Connect-macOS-Installer


echo "Packaging finished."
echo "The installer is ADB-Wireless-Connect-macOS-Installer"
pause

