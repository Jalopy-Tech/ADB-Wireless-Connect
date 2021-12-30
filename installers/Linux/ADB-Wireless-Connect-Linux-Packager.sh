#!/bin/bash
echo "ADB Wireless Connect Linux Packager"

echo "Deleting old installer..."
rm -r DistributionKit
rm ADB-Wireless-Connect-Linux-Installer*

echo "Creating new installer..."
cqtdeployer -confFile "./configFile.json"

echo "Moving new installer..."
mv DistributionKit/ADB-Wireless-Connect-Linux-Installer* .

echo "Deleting temporary files..."
rm -r DistributionKit

echo "Packaging finished."
echo "The installer is ADB-Wireless-Connect-Linux-Installer.run"

read -p "Press [Enter] key"
