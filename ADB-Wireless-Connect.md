# ADB Wireless Connect

### Overview

<img align="right" src="./images/ADB Wireless Connect.png" alt="ADB Wireless Connect" style="zoom:100%;" />ADB Wireless Connect is a GUI front-end to to connect or pair an Android device using adb.

 The Android device needs to connected the same network via WiFi. The connecting device needs to be connected using either WiFI or Ethernet.

### Installation

ADB Wireless Connect installation instructions are on the main <a href="https://github.com/Jalopy-Tech/ADB-Wireless-Connect/blob/main/README.md">README</a> page.

### ADB Tools

ADB Tools needs to be installed and the adb command needs to be in the PATH. More infromation can be found in the installtion instructions. When the app is opened, the adb location is displayed.

To check if the adb command is available and in the PATH, open up a Terminal/Command prompt and type in:

  ```
  adb devices
  ```

 A list of connected devices will be displayed (this will probably be empty).

To chek if the adb version includes the pair option, open up a Terminal/Command prompt and type in:

  ```
adb pair
  ```

 If abd reports "Unknown command", it needs updating to the latest version (see the Installation section).

### Paring

An Android device first needs to be be paired before it can be connected. In the app, when the pair action is selected, the "Pair Instructions" button will be available to display a quick set of instructions.

The port used for pairing is only used for pairing and is not related the port used for connecting.

### Connecting

In the app, when the connect action is selected, the "Connect Instructions" button will be available to display a quick set of instructions.

The port used for connecting is not related the port used for pairing.

If the Android device is going to be regulary conected, the app can remember the IP address and port for subsequent connections.  Unfortunatley Android uses a new port when the device is rebooted, so remembering the port may not be useful. However, the IP address can be made static, either by setting up the Android device with a static IP address, or using the router's function to assign the same IP address to the Android device every time by storing its MAC address.

### Command line options

The app can pass in options when it is called.

Usage: ./adb-wireless-connect [options]

Options:

  -h, --help                        Displays help on commandline options.

  --help-all                        Displays help including Qt specific options.

  -v, --version                     Displays version information.

  -i, --ip-address <ipAddress>      Specify the IP address.

  -c, --connect-port <connectPort>  Specify the connect port.

  -p, --pair-port <pairPort>        Specify the pair port.

### License

Copyright &copy; 2022 Robert J. Joynt.

The <a href="https://github.com/Jalopy-Tech/ADB-Wireless-Connect /blob/main/LICENSE.md">ADB Wireless Connect  license</a> uses the <a href="https://www.qt.io/">QT</a> GPLv3 <a href="https://doc.qt.io/qt-5/gpl.html">Open Source License</a>