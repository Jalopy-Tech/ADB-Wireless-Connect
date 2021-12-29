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


    This program uses a MVP (Model-View-Presenter) architecture using a supervising presenter.

    There are two views: Gui and Console. Each View instantiates a Presenter to carry
    out the view logic. The Presenter uses view independent logic: both Views use the
    same Presenter. The View signals to the Presenter when a task is needed
    and the Presenter calls View methods to provide the results.

    The controller reads in the command arguments and then parses the command line. It
    instatiates the appropiate view acting as a view factory. It also finds the
    external command needed by the presenter.

    The Model provides application logic for the Presenter.
*/

#include "controller.h"
#include "view.h"


#include <QApplication>

#include <QDebug>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Jalopy Software");
    QCoreApplication::setApplicationName("ADB Wireless Connect");
    QCoreApplication::setApplicationVersion("1.0");

    Controller controller;

    View *view;
    view = controller.view();

    int returnCode = 0;

    if (view != NULL)
        returnCode = view->exec();
    else
        returnCode = 1;

    return returnCode;

}

