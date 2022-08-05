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

   ------------------------------------------------------------------------------------

    The List Interpreter is a subclass of Interpreter. It interprets output from
    "adb devices -l"
*/


#ifndef LIST_INTERPRETER_H
#define LIST_INTERPRETER_H

#include "interpreter.h"

#include <QObject>
#include <QDebug>

class ListInterpreter : public Interpreter
{
    Q_OBJECT

public:
    explicit        ListInterpreter(QObject *parent = nullptr);
    void            interpret(const QString& line) override;

    CommandType     commandType() const override;
    QString         commandTitle() const override;

signals:
    void            deviceListed(const QString& device, const QString& qualifiers);
    void            listingDevicesReported(const QString& message);

private:
    bool            _isExpectingDevices = false;

};

#endif

