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

    The Presenter carries out tasks requested by the view. It uses view independent
    logic: both Views use this same Presenter. The View signals to the Presenter when
    a task is needed and the Presenter calls View methods to provide the results.
*/

#ifndef PRESENTER_H
#define PRESENTER_H

#include "view.h"
#include "constants.h"

#include "interpreter.h"
#include "listinterpreter.h"
#include "connectinterpreter.h"
#include "pairinterpreter.h"
#include "disconnectinterpreter.h"
#include "disconnectallinterpreter.h"


#include <QObject>
#include <QCommandLineParser>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QProcess>


class Presenter : public QObject
{
    Q_OBJECT

public:
    explicit Presenter(View* view, const QString& ipAddress,
                       const QString& connectPort,
                       const QString& pairPort,
                       const QString& adbCommand,
                       QObject *parent = nullptr);

    ~Presenter();

    View            *view;
    Interpreter     *interpreter = NULL;

public slots:
    void            handleAdbStarted();
    void            handleAdbReadReady();
    void            handleAdbErrorOccurred(const QProcess::ProcessError& error);
    void            handleAdbFinished(int exitCode, const QProcess::ExitStatus& exitStatus);
    void            handleAdbProgressed(int progress, const QString& progressText);

    void            adbList();

    void            adbConnect(const QString& ipAddress,
                               bool fRememberIpAddress,
                               const QString& connectPort,
                               bool fRememberConnectPort);

    void            adbPair(const QString& ipAddress,
                            bool fRememberIpAddress,
                            const QString& pairPort,
                            const QString& pairCode);

    void            adbDisconnect(const QString& device);

    void            adbDisconnectAll();

    void            adbStop();

private:

    QString         replaceAppConstantTags(const QString& s) const;

    QSettings       _settings;

    QString         _ipAddress;
    bool            _fRememberIpAddress;
    QString         _connectPort;
    bool            _fRememberConnectPort;
    QString         _pairPort;

    QString         _parsedIpAddress;
    QString         _parsedConnectPort;
    QString         _parsedPairPort;
    QString         _pairCode;

    QString         _adbCommand;
    QString         _nativeAdbProcessCommand;
    QProcess        _adbProcess;

    QString         _connectInstructionsText;
    QString         _pairInstructionsText;
    QString         _aboutText;

    int             _progress;
    bool            _isProcessStopping = false;

};

#endif
