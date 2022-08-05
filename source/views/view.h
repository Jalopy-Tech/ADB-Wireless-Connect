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

   This is the abstract view class. All concrete views extend from this class.
   Each view has a presenter to carry out the view presentation logic.
*/

#ifndef VIEW_H
#define VIEW_H

//#include "presenter.h"
#include <QObject>

class Presenter;

class View : public QObject
{
    Q_OBJECT


public:
    explicit        View(QObject *parent = nullptr);
    ~View();

    virtual int     exec() = 0;

    virtual void    init(const QString& ipAddress,
                         const QString& connectPort,
                         const QString& pairPort,
                         const QString& adbCommand) = 0;

    void            setExecutable(bool isExecutable);
    bool            executable();

    virtual void    showError(const QString& msg) = 0;
    virtual void    showInformation(const QString& msg) = 0;


    virtual void    setIpAddress(const QString& ipAddress) = 0;
    virtual QString ipAddress() const = 0;

    virtual void    setConnectPort(const QString& connectPort) = 0;
    virtual QString connectPort() const = 0;

    virtual void    setPairPort(const QString& pairPort) = 0;
    virtual QString pairPort() const = 0;

    virtual void    setPairCode(const QString& pairCode) = 0;
    virtual QString pairCode() const = 0;


    virtual void    setfRememberIpAddress(bool fRememberIpAddress) = 0;
    virtual bool    fRememberIpAddress() const = 0;

    virtual void    setfRememberConnectPort(bool fRememberConnectPort) = 0;
    virtual bool    fRememberConnectPort() const = 0;

    virtual QString connectedDevice() const = 0;

    virtual void    setAdbCommand(const QString& adbComamnd) = 0;
    virtual QString adbCommand() const = 0;

    virtual void    setConnectInstructions(const QString& text) = 0;
    virtual void    setPairInstructions(const QString& text) = 0;
    virtual void    setAbout(const QString& text) = 0;

    virtual void    appendLog(const QString& logText) = 0;
    virtual void    clearLog() = 0;
    virtual QString getAdbProcessCommand(const QString& applicationFilePath) = 0;
    virtual void    enableActions() = 0;
    virtual void    disableActions() = 0;
    virtual void    setProgress(int progress) = 0;

    virtual void    clearConnectedDevices() = 0;

signals:
    void            adbListRequested();

    void            adbConnectRequested(QString ipAddress,
                                        bool fRememberIpAddress,
                                        QString connectPort,
                                        bool fRememberConnectPort);

    void            adbPairRequested(QString ipAddress,
                                     bool fRememberIpAddress,
                                     QString pairPort,
                                     QString pairCode);

    void            adbDisconnectRequested(QString device);

    void            adbDisconnectAllRequested();

    void            adbStopRequested();

public slots:

    virtual void    addConnectedDevice(const QString& device, const QString& qualifiers) = 0;
    virtual void    informListingDevicesReported(const QString& message) = 0;
    virtual void    informDeviceConnected(const QString& message) = 0;
    virtual void    informDeviceAlreadyConnected(const QString& message) = 0;
    virtual void    informConnectingDeviceFailed(const QString& message) = 0;
    virtual void    informConnectingDeviceReported(const QString& message) = 0;
    virtual void    informDevicePaired(const QString& message) = 0;
    virtual void    informPairingDeviceFailed(const QString& message) = 0;
    virtual void    informPairingDeviceReported(const QString& message) = 0;
    virtual void    informDeviceDisconnected(const QString& message) = 0;
    virtual void    informDisconnectingDeviceFailed(const QString& message) = 0;
    virtual void    informDisconnectingDeviceReported(const QString& message) = 0;
    virtual void    informAllDevicesDisconnected(const QString& message) = 0;
    virtual void    informDisconnectingAllDevicesReported(const QString& message) = 0;

protected:
    Presenter       *presenter = NULL;

private:
    bool            _isExecutable = true;
};


#endif

