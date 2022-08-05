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

    The Interpreter is an abstract class. Any concrete subclass interprets the output
    of comamnds.
*/


#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>
#include <QDebug>

class Interpreter : public QObject
{
    Q_OBJECT

public:
    enum CommandType {NO_COMMAND, LIST_COMMAND, CONNECT_COMMAND,
                      PAIR_COMMAND, DISCONNECT_COMMAND, DISCONNECT_ALL_COMMAND
                     };

    explicit        Interpreter(QObject *parent = nullptr);



    virtual void    interpret(const QString& line) = 0;

    virtual CommandType
    commandType() const = 0;
    virtual QString commandTitle() const = 0;

};


#endif

