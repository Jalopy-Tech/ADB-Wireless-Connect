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

#include "listinterpreter.h"

ListInterpreter::ListInterpreter(QObject *parent)
    : Interpreter(parent)
{

}

Interpreter::CommandType ListInterpreter::commandType() const
{
    return Interpreter::LIST_COMMAND;
}

QString ListInterpreter::commandTitle() const
{
    return "List command";
}

void ListInterpreter::interpret(const QString& line)
{

    QString searchString;

    if(line.isEmpty())
        return;

    if(_isExpectingDevices) {
        QString device;

        device = line.split(" ").at(0);
        QString qualifiers = line.right(line.size() - device.size() - 1);

        emit deviceListed(device, qualifiers);

        return;
    }

    searchString = "list of devices attached";

    if(line.toLower().startsWith(searchString.toLower())) {
        _isExpectingDevices = true;
        return;
    }

    searchString = "starting now";

    if(line.toLower().contains(searchString.toLower())) {
        return;
    }

    searchString = "daemon started";

    if(line.toLower().contains(searchString.toLower())) {
        return;
    }


    emit listingDevicesReported(line);
    return;

}
