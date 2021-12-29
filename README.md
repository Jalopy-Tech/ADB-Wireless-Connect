# ADB Wireless Connect

## Overview

<img align="right" src="/mnt/Data/Projects/ADB Wireless Connect/images/ADB Wireless Connect.png" alt="ADB Wireless Connect" style="zoom:100%;" />ADB Wireless Connect is a GUI front-end to to connect or pair an Android device using adb. It is available in Linux, Windows and macOS. 

ADB Wireless Connect application instructions are on the <a href="https://github.com/Jalopy-Tech/ADB-Wireless-Connect/blob/main/ADB-Wireless-Connect.md">ADB Wireless Connect</a> page.

The app relies on ADB Tools being installed with the adb executable file in the search PATH. Instructions to install ADB Tools in your particular operating system will be available on the web. A simple and common method of installing ADB Tools for Linux, Windows and macOS is given below.

### Installation

#### Linux

<img align="right" src="./images/Linux.png" alt="Linux" style="zoom:33%;" />In the latest release, download the executable file "ADB-Wireless-Connect-Linux-Installer.run" and run it in Linux. This installer will ask for the destination folder to be created. It defaults to a new folder ("adb-wireless-connect") in the home folder. The app can also be installed to a root folder such as "/opt/adb-wireless-connect" as desired (root password will be needed).. The app files will be installed and a shortcut will be added to the menu under "ADB Wireless Connect". The app can be run directly using the "adb-wireless-connect.sh" file in the installation folder.

To uninstall, go to the installation folder and run the executable file "maintenancetool".

ADB Tools needs to be installed and in the search PATH. ADB Tools may be available in your distribution's software manager.

- Debian-based Linux command:

  ```
  sudo apt-get install android-tools-adb
  ```

- Fedora/SUSE-based Linux command:

  ```
  sudo yum install android-tools
  ```

Be aware that this may not be the lasted version of ADB Tools. This is especially relevant if the version does not support the adb pair command. To install the latest version manually, see the following guide: https://www.xda-developers.com/install-adb-windows-macos-linux/ . Make sure the adb is in your PATH. This can be set in your .profile file.

####  Windows

<img align="right"  src="./images/Windows.png" alt="Windows" style="zoom: 33%;" /> In the latest release, download the executable file "ADB-Wireless-Connect-Windows-Installer.exe" and run it in Windows. This installer will install ADB Wireless Connect. It will add a shortcut to the Windows start menu under "ADB Wireless Connect" and add a shortcut to the desktop.

To uninstall, go to the Control Panel and uninstall ADB Wireless Connect. To uninstall manually, go to the installation folder and run the executable file "maintenancetool.exe".

To install ADB Tools in Windows, do the following:

1. Download the [Android SDK Platform Tools ZIP file for Windows](https://dl.google.com/android/repository/platform-tools-latest-windows.zip).
2. Extract the contents of this ZIP file into an easily accessible folder.e.g. "C:\platform-tools".
3. Press the Windows key and search for "Edit the system environent variables" and open up this Control Panel app.
4. In the "Advanced" page tab, click the "Environment variabes" button.
5. In the "User variables" for your user section, highlight the "Path" variable and click the "Edit" button.
6. Click the "New" button and then click the "Browse" button to add a new folder to the path.
7. Select the folder where you put the downloaded ADB Tools folder. e.g. "C:\platform-tools".
8. Log out or restart Windows to allow the path to be reset.

#### macOS

<img align="right" src="./images/macOS.png" alt="macOS" style="zoom:33%;" />In the latest release, download the ADB-Wireless-Connect-macOS-Image.dmg file and open it in macOS. The image will mount showing the "ADB Wireless Connect.app" application. This can be dragged into the Applications folder using Finder and will be available from the Launcher. Alternatively the app can be opened immediately to run the app.

To uninstall, use Finder to view the "ADB Wireless Connect.app" in the Applications folder. Delete this file.

ADB Tools can be installed using  Homebrew. Open the Terminal app, and enter the following command:

```bash
brew install --cask android-platform-tools
```

Be aware that this may not be the lasted version of ADB Tools. This is especially relevant if the version does not support the adb pair command. To install the latest version manually, see the following guide: https://www.xda-developers.com/install-adb-windows-macos-linux/ . Make sure the adb is in your PATH. This can be set in your .profile file.

### License

Copyright &copy; 2022 Robert J. Joynt.

The <a href="https://github.com/Jalopy-Tech/ADB-Wireless-Connect/blob/master/LICENSE.md">ADB Wireless Connect license</a> uses the <a href="https://www.qt.io/">QT</a> GPLv3 <a href="https://doc.qt.io/qt-5/gpl.html">Open Source License</a>

### Contact

For further inquires, please email: robertjjoynt@gmail.com .











