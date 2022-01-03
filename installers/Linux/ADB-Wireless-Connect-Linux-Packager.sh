#!/bin/bash
echo "ADB Wireless Connect Linux Packager"

echo "Deleting old installer..."
rm ADB-Wireless-Connect-Linux-Installer*

echo Creating Linux deployable folder... 
rm -R "adb-wireless-connect"
mkdir "adb-wireless-connect"

echo "Creating new installer..."
cqtdeployer -confFile "./configFile.json"

echo "Moving new installer..."
mv adb-wireless-connect/ADB-Wireless-Connect-Linux-Installer* .

echo "Deleting temporary files..."
rm -R "adb-wireless-connect"

echo "Packaging finished."
echo "The installer is ADB-Wireless-Connect-Linux-Installer.run"

read -p "Press [Enter] key"
