#!/bin/bash
echo "ADB Wireless Connect Linux Packager"

echo "Deleting old installer..."
rm -r DistributionKit
rm ADB-Wireless-Connect-Linux-Installer*

echo "Creating new installer..."
cqtdeployer -confFile "./configFile.json"

echo "Moving new installer..."
mv DistributionKit/ADB-Wireless-Connect-Linux-Installer* .

rm -r DistributionKit

#echo "Deleting old app image..."
#rm -r ADB-Wireless-Connect*AppImage

#echo "Creating new app image..."
#~/linuxdeployqt/linuxdeployqt-continuous-x86_64.AppImage "../../builds/Linux/release/adb-wireless-connect/adb-wireless-connect" -appimage

echo "Packaging finished."
echo "The installer is ADB-Wireless-Connect-Linux-Installer.run"
#echo "The app image is ADB-Wireless-Connect-Linux-Installer.AppImage"
read -p "Press [Enter] key"
