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

   This is the abstract view class. All concrete views extend from this class.
   Each view has a presenter to carry out the view presentation logic.
*/

#ifndef VIEW_H
#define VIEW_H


#include <QObject>


class Presenter;

class View : public QObject
{
    Q_OBJECT


public:
    explicit View(QObject *parent = nullptr);
    ~View();

    virtual int exec() = 0;

    virtual void init(const QString& ipAddress,
                      const QString& connectPort, const QString& pairPort) = 0;

    void setExecutable(bool isExecutable);
    bool executable();

    virtual void showError(const QString& msg) = 0;
    virtual void showInformation(const QString& msg) = 0;


    virtual void setIpAddress(const QString& ipAddress) = 0;
    virtual QString ipAddress() = 0;

    virtual void setConnectPort(const QString& connectPort) = 0;
    virtual QString connectPort() = 0;

    virtual void setPairPort(const QString& pairPort) = 0;
    virtual QString pairPort() = 0;

    virtual void setPairCode(const QString& pairCode) = 0;
    virtual QString pairCode() = 0;

    virtual void setParsedIpAddress(const QString& parsedIpAddress) = 0;
    virtual QString parsedIpAddress() = 0;

    virtual void setParsedConnectPort(const QString& parsedConnectPort) = 0;
    virtual QString parsedConnectPort() = 0;

    virtual void setParsedPairPort(const QString& parsedPairPort) = 0;
    virtual QString parsedPairPort() = 0;

    virtual void setfRememberIpAddress(bool fRememberIpAddress) = 0;
    virtual bool fRememberIpAddress() = 0;

    virtual void setfRememberConnectPort(bool fRememberConnectPort) = 0;
    virtual bool fRememberConnectPort() = 0;

    virtual void setAdbCommand(const QString& adbComamnd) = 0;
    virtual QString  adbCommand() = 0;

    virtual void setConnectInstructionText(const QString& instructionText) = 0;
    virtual void setPairInstructionText(const QString& instructionText) = 0;

    virtual void    appendLog(const QString& logText) = 0;

    virtual QString getAdbProcessCommand(const QString& applicationFilePath) = 0;
    virtual void    startAction() = 0;
    virtual void    finishAction() = 0;
    virtual void    setProgress(int progress) = 0;


signals:
    void            adbConnectRequested();
    void            adbPairRequested();
    void            adbStopRequested();


protected:
    Presenter *presenter = NULL;

private:
    bool _isExecutable = true;
};


#endif

