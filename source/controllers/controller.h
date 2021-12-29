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

   ------------------------------------------------------------------------------------

    The controller reads in the command arguments and then parses the command line. It
    instatiates the appropiate view acting as a view factory. It also finds the
    external command needed by the presenter.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"
#include "gui_view.h"


#include <QObject>
#include <QStandardPaths>
#include <QSettings>

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    View* view();

private:
    void setAdbFilePath();
    View *_view = NULL;
    bool _isValid = true;

    QString _adbFilePath;
    QString _ipAddress;
    QString _connectPort;
    QString _pairPort;
    QString _parsedIpAddress;
    QString _parsedConnectPort;
    QString _parsedPairPort;
    QString _settingsIpAddress;
    QString _settingsConnectPort;
    bool _fRememberIpAddress;
    bool _fRememberConnectPort;



};

#endif
