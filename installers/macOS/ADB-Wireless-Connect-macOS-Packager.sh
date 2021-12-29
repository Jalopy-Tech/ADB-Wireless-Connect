#!/bin/bash
echo "ADB Wireless Connect macOS Packager"

echo "Deleting old installer..."
rm "./ADB-Wireless-Connect-macOS-Image.dmg"

echo "Creating new installer..."
~/qt/6.2.2/macos/bin/macdeployqt "../../builds/macOS/release/adb-wireless-connect/ADB Wireless Connect.app" -dmg

echo "Moving new installer..."
mv "../../builds/macOS/release/adb-wireless-connect/ADB Wireless Connect.dmg" "./ADB-Wireless-Connect-macOS-Image.dmg"


echo "Packaging finished."
echo "The package is ADB-Wireless-Connect-macOS-Image.dmg"

read -p "Press [Enter] key"
