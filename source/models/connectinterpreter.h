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

    The Connect Interpreter is a subclass of Interpreter. It interprets output from
    "adb connect"
*/


#ifndef CONNECT_INTERPRETER_H
#define CONNECT_INTERPRETER_H

#include "interpreter.h"

#include <QObject>
#include <QDebug>

class ConnectInterpreter : public Interpreter
{
    Q_OBJECT

public:
    explicit        ConnectInterpreter(QObject *parent = nullptr);
    void            interpret(const QString& line) override;

    CommandType     commandType() const override;
    QString         commandTitle() const override;

signals:
    void            deviceConnected(const QString& message);
    void            deviceAlreadyConnected(const QString& message);
    void            connectingDeviceFailed(const QString& message);
    void            connectingDeviceReported(const QString& message);
};

#endif

