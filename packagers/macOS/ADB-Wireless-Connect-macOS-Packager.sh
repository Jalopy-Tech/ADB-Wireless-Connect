#!/bin/bash
echo "ADB Wireless Connect macOS Packager" 

echo "Deleting old installer..."
rm -R "./ADB-Wireless-Connect-macOS-Installer.dmg"
rm -R "./ADB-Wireless-Connect-macOS-Installer.app"
rm -R "./packages/com.adbwirelessconnect/data/ADB Wireless Connect.app" 

echo "Copying application for making deployable..." 
rm -R "./packages/com.adbwirelessconnect/data/*"
cp -R "../../builds/macOS/release/adb-wireless-connect/ADB Wireless Connect.app" "./packages/com.adbwirelessconnect/data"

echo Creating macOS deployable app using macdeployqt...
~/Qt/6.3.1/macos/bin/macdeployqt "./packages/com.adbwirelessconnect/data/ADB Wireless Connect.app"


echo "Creating installer..."
~/Qt/Tools/QtInstallerFramework/4.4/bin/binarycreator --offline-only  -c config/config.xml -p packages ADB-Wireless-Connect-macOS-Installer

echo "Creating disk image of installer using macdeployqt..."
~/Qt/6.3.1/macos/bin/macdeployqt "ADB-Wireless-Connect-macOS-Installer.app" -dmg
rm -R "ADB-Wireless-Connect-macOS-Installer.app" 

echo Deleting temporary packages...
rm -R "./packages/com.adbwirelessconnect/data/ADB Wireless Connect.app"

echo "Packaging finished."





echo "The installer is ADB-Wireless-Connect-macOS-Installer"
read -p "Press [Enter] key"

