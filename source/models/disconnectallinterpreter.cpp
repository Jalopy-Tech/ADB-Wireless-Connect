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

#include "disconnectallinterpreter.h"

DisconnectAllInterpreter::DisconnectAllInterpreter(QObject *parent)
    : Interpreter(parent)
{

}

Interpreter::CommandType DisconnectAllInterpreter::commandType() const
{
    return DISCONNECT_ALL_COMMAND;
}

QString DisconnectAllInterpreter::commandTitle() const
{
    return "Disconnect All command";
}

void DisconnectAllInterpreter::interpret(const QString& line)
{
    QString searchString;

    if(line.isEmpty())
        return;

    searchString = "disconnected everything";

    if(line.toLower().startsWith(searchString.toLower())) {
        QString message = tr("All devices disconnected: ") + line;
        emit allDevicesDisconnected(message);
        return;
    }

    emit disconnectingAllDevicesReported(line);
    return;


}
