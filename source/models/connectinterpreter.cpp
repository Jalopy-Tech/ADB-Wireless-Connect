/*
    Copyright (C) 2022 Robert J. Joynt.

    This software uses the QT (https://www.qt.io/) GPLv3 Open Source License
    (https://doc.qt.io/qt-6/gpl.html).

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

#include "connectinterpreter.h"

ConnectInterpreter::ConnectInterpreter(QObject *parent)
    : Interpreter(parent)
{

}

Interpreter::CommandType ConnectInterpreter::commandType() const
{
    return Interpreter::CONNECT_COMMAND;
}

QString ConnectInterpreter::commandTitle() const
{
    return "Connect command";
}

void ConnectInterpreter::interpret(const QString& line)
{
    QString searchString;

    if(line.isEmpty())
        return;

    searchString = "failed";

    if(line.toLower().startsWith(searchString.toLower())) {
        QString message =  tr("Connecting to device failed: ") + line;
        emit connectingDeviceFailed(message);
        return;
    }

    searchString = "already connected";

    if(line.toLower().startsWith(searchString.toLower())) {
        QString message = tr("Device already connected: ") + line;
        emit deviceAlreadyConnected(message);
        return;
    }

    searchString = "connected to ";

    if(line.toLower().startsWith(searchString.toLower())) {
        QString message = tr("Device connected: ") + line;
        emit deviceConnected(message);
        return;
    }

    emit connectingDeviceReported(line);
    return;

}
