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

   This class extends the abstract View class to create a concrete view
   for a GUI interface. It has a presenter to carry out the view presentation
   logic.
*/

#ifndef GUI_VIEW_H
#define GUI_VIEW_H

#include "view.h"
#include "presenter.h"
#include "ui_mainwindow.h"
#include "ui_connectdialog.h"
#include "ui_pairdialog.h"


#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QSettings>
#include <QTableWidget>

class Presenter;

class GuiView : public View
{
    Q_OBJECT

public:
    explicit        GuiView(QObject *parent = nullptr);
    ~GuiView();

    int             exec() override;

    void            init(const QString& ipAddress,
                         const QString& connectPort,
                         const QString& pairPort,
                         const QString& adbCommand) override;

    void            showError(const QString& msg) override;
    void            showInformation(const QString& msg) override;

    void            appendLog(const QString& logText) override;
    void            clearLog() override;
    void            showLog();
    void            hideLog();
    void            toggleShowOrHideLog();

    void            setIpAddress(const QString& ipAddress) override;
    QString         ipAddress() const override;

    void            setConnectPort(const QString& connectPort) override;
    QString         connectPort() const override;

    void            setPairPort(const QString& pairPort) override;
    QString         pairPort() const override;

    void            setPairCode(const QString& pairCode) override;
    QString         pairCode() const override;


    void            setfRememberIpAddress(bool fRememberIpAddress) override;
    bool            fRememberIpAddress() const override;

    void            setfRememberConnectPort(bool fRememberConnectPort) override;
    bool            fRememberConnectPort() const override;

    QString         connectedDevice() const override;

    void            setAdbCommand(const QString& adbCommand) override;
    QString         adbCommand() const override;

    void            setConnectInstructions(const QString& text) override;
    void            setPairInstructions(const QString& text) override;
    void            setAbout(const QString& text) override;

    void            setProgress(int progress) override;
    void            startProgress();

    QString         getAdbProcessCommand(const QString& applicationFilePath) override;
    void            enableActions() override;
    void            disableActions() override;

    void            clearConnectedDevices() override;

public slots:
    void            addConnectedDevice(const QString& device, const QString& qualifiers) override;
    void            informListingDevicesReported(const QString& message) override;
    void            informDeviceConnected(const QString& message) override;
    void            informDeviceAlreadyConnected(const QString& message) override;
    void            informConnectingDeviceFailed(const QString& message) override;
    void            informConnectingDeviceReported(const QString& message) override;
    void            informDevicePaired(const QString& message) override;
    void            informPairingDeviceFailed(const QString& device) override;
    void            informPairingDeviceReported(const QString& message) override;
    void            informDeviceDisconnected(const QString& message) override;
    void            informDisconnectingDeviceFailed(const QString& message) override;
    void            informDisconnectingDeviceReported(const QString& message) override;
    void            informAllDevicesDisconnected(const QString& message) override;
    void            informDisconnectingAllDevicesReported(const QString& message) override;

private slots:
    void            help();
    void            exit();

private:

    Ui::MainWindow  mainWindowUi;
    QMainWindow     mainWindow;

    Ui::ConnectDialog
    connectDialogUi;
    QDialog         connectDialog;

    Ui::PairDialog  pairDialogUi;
    QDialog         pairDialog;

    void            changeConnectAbility();
    void            changePairAbility();
    void            changeDisconnectAbility();

    void            setForegroundToBackground(QWidget *widget);

    void            resizeButtonBoxButtons();

    void            doList();
    void            doConnect();
    void            doPair();
    void            doDisconnect();
    void            doDisconnectAll();

    void            stopAction();
    void            showConnectDialog();
    void            showPairDialog();

    void            showConnectInstructions();
    void            showPairInstructions();
    void            showAbout();

    bool            _isDoingAction = false;

    QString         _ipAddress;
    bool            _fRememberIpAddress;
    QString         _connectPort;
    bool            _fRememberConnectPort;
    QString         _pairPort;
    QString         _pairCode;

    QString         _adbCommand;
    QString         _connectInstructionsText;
    QString         _pairInstructionsText;
    QString         _aboutText;

    QSettings       _settings;
    int             _originalHeight;
    bool            _isVisibleLog;
    int             _logTextBrowserHeight = 0;

    QPushButton     *_focusedPushButton = NULL;
};

#endif
