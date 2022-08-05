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

    Unforuntately Windows doesn't allow an application to easily act as both a GUI
    application and as a command line (console) application. The safest way to deal
    with this, is to have two appications, one for GUI and one for command line and
    for a robust solution, this is the recommended solution.

    This namespace has routines that will provide a rudimetary alternative solution for
    basic command line use. It is far from perfect as display problems such as extra
    prompts and line break formatting show up. There will also be problems using
    pipe commands and other redirection uses. In addition, using standard input seems
    to be broken.

    Including and using this namepsace won't affect other operating systems due to the #ifdef
    usage.

    To use these routines, call WindowsConsoleFix::initialize() to start console
    operations and WindowsConsoleFix::finalize() to finish it. These methods can be called
    in the constructor and destructor methods of the view class respectively to handle the console
    interface.
*/

#ifdef _WIN32
    #include <iostream>
    #include <Windows.h>
#endif

#ifndef WINDOWSCONSOLEFIX_H
#define WINDOWSCONSOLEFIX_H


namespace WindowsConsoleFix
{
    void            initialize();
    void            finalize();
};

#endif

