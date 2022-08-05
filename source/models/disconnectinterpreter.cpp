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

#include "disconnectinterpreter.h"

DisconnectInterpreter::DisconnectInterpreter(QObject *parent)
    : Interpreter(parent)
{

}

Interpreter::CommandType DisconnectInterpreter::commandType() const
{
    return DISCONNECT_COMMAND;
}

QString DisconnectInterpreter::commandTitle() const
{
    return "Disconnect command";
}

void DisconnectInterpreter::interpret(const QString& line)
{
    QString searchString;

    if(line.isEmpty())
        return;

    searchString = "error";

    if(line.toLower().startsWith(searchString.toLower())) {
        QString message = tr("Disconnecting device failed: ") + line;

        emit disconnectingDeviceFailed(message);
        return;
    }

    searchString = "disconnected";

    if(line.toLower().startsWith(searchString.toLower())) {
        QString device = line.right(line.size() - searchString.size());

        QString message = tr("Device disconnected: ") + line;

        emit deviceDisconnected(message);
        return;
    }

    emit disconnectingDeviceReported(line);
    return;
}
