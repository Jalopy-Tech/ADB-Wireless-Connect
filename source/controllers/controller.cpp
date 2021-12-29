/*
    Copyright (C) 2022 Robert J. Joynt.

    This software uses the QT (https://www.qt.io/) GPLv3 Open Source License
    (https://doc.qt.io/qt-5/gpl.html).

    This software is distributed under the GNU General Public License Version 3.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    (https://www.gnu.org/licenses/).
*/

#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject(parent)
{
    QCommandLineParser parser;

  // Set up the parser.

    parser.setApplicationDescription(tr("A GUI front end to connect and pair using adb."));

     // Set up the various command options allowed in the parser.

    const QCommandLineOption helpOption = parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();

    const QCommandLineOption ipAddressOption(QStringList() << "i" << "ip-address",
           tr("Specify the IP address."), "ipAddress");
    parser.addOption(ipAddressOption);

    const QCommandLineOption connectPortOption(QStringList() << "c" << "connect-port",
          tr("Specify the connect port."), "connectPort");
    parser.addOption(connectPortOption);

    const QCommandLineOption pairPortOption(QStringList() << "p" << "pair-port",
          tr("Specify the pair port."), "pairPort");
    parser.addOption(pairPortOption);


    // Parce the parser to extract all the command line options.

    bool fParseError = !parser.parse(QCoreApplication::arguments());

    // Assign all the command line options to variables.

    bool hasHelpOption = parser.isSet(helpOption);
    bool hasVersionOption = parser.isSet(versionOption);

    _parsedIpAddress = parser.value(ipAddressOption);
    _parsedConnectPort = parser.value(connectPortOption);
    _parsedPairPort = parser.value(pairPortOption);


    // construct the view


    _view = new GuiView();

    // Check if the parser had errors or requested help or verion

    if (fParseError) {
        _view->showError(parser.errorText());
        _view->setExecutable(false);
    } else if (hasHelpOption) {
         _view->showInformation(parser.helpText());
         _view->setExecutable(false);
    } else if (hasVersionOption) {
            QString versionText = QString("") + \
           QCoreApplication::applicationName() + " " + \
           QCoreApplication::applicationVersion();

       _view->showInformation(versionText);
       _view->setExecutable(false);
    } else {

        _view->init(_parsedIpAddress, _parsedConnectPort, _parsedPairPort);

        setAdbFilePath();
        _view->setAdbCommand(_adbFilePath);



    }
}


Controller::~Controller()
{
    delete (_view);
}

View* Controller::view()
{
    return _view;
}

void Controller::setAdbFilePath() {

    bool fFound = false;
    QStringList filters;
    filters << "adb";



    // Check if the adb command is in the application directory or one of its
    // children

    if (!fFound) {
         QStringList paths;

        // Add all the application directory itself to the seach path

        paths << QCoreApplication::applicationDirPath();


        // Add all the children dirs to the search paths

        QDir dir(QCoreApplication::applicationDirPath());

        dir.setNameFilters(filters);

        QFileInfoList dirList;
        dirList = dir.entryInfoList(filters, QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
        if (!dirList.isEmpty())
           for( int i = 0 ; i < dirList.count(); ++i )
                paths << dirList[i].absoluteFilePath();


       // Search the paths that were set to find the command

        QString filePath = QStandardPaths::findExecutable("adb", paths);


        if (filePath != "") {
            _adbFilePath = filePath;
            fFound = true;
        }
    }

    // Search for the adb command in the standard paths set by the operating system

    if (!fFound) {
      QString filePath = QStandardPaths::findExecutable("adb");

      if (filePath != "") {
          _adbFilePath = filePath;
          fFound = true;
      }
    }

    // The findExecutable function doesn't search in /usr/local/bin, etc on macOS, so do a manual search

     #ifdef Q_OS_MACX
     if (!fFound) {
        QStringList dirList;
        dirList.append("/usr/local/bin");
        dirList.append("/usr/local/sbin");
        dirList.append("/usr/bin");
        dirList.append("/usr/sbin");

        QString filePath = QStandardPaths::findExecutable("adb",dirList);

      if (filePath != "") {
          _adbFilePath = filePath;
          fFound = true;
      }
    }
    #endif


    // adb command not found, so show error

    if (!fFound)
       {
        QString appPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
        QString msg = QString("") + tr("The executable command \"adb\" cannot be found.") + \
                tr(" It needs to be in:\n\n") + \
                tr("  This application's folder,\n") + \
                tr("\n    OR \n\n") + \
                tr("  A subfolder of this application's folder,\n") + \
                tr("\n    OR \n\n") + \
                tr("  The standard paths of the operating system.") + \
                tr("\n\n(Note that this application's folder is:\n") + appPath + " )" + \
                "";


        _view->showError(msg);
        _view->setExecutable(false);


        }

    if (!fFound)
        _isValid = false;
}





