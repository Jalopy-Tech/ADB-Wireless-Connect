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

   This class extends the abstract View class to create a concrete view
   for a GUI interface. It has a presenter to carry out the view presentation
   logic.
*/

#ifndef GUI_VIEW_H
#define GUI_VIEW_H

#include "view.h"
#include "presenter.h"
#include "ui_main_dialog.h"
#include "ui_action_dialog.h"

#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>

class Presenter;

class GuiView : public View
{
    Q_OBJECT


public:
    explicit        GuiView(QObject *parent = nullptr);

    ~GuiView();

    int exec() override;

    void init(const QString& ipAddress, const QString& connectPort,
              const QString& pairPort) override;


    void showError(const QString& msg) override;
    void showInformation(const QString& msg) override;

    void            appendLog(const QString& logText) override;


    void setIpAddress(const QString& ipAddress) override;
    QString ipAddress() override;

    void setConnectPort(const QString& connectPort) override;
    QString connectPort() override;

    void setPairPort(const QString& pairPort) override;
    QString pairPort() override;

    void setPairCode(const QString& pairCode) override;
    QString pairCode() override;

    void setParsedIpAddress(const QString& parsedIpAddress) override;
    QString parsedIpAddress() override;

    void setParsedConnectPort(const QString& parsedConnectPort) override;
    QString parsedConnectPort() override;

    void setParsedPairPort(const QString& parsedPairPort) override;
    QString parsedPairPort() override;

    void setfRememberIpAddress(bool fRememberIpAddress) override;
    bool fRememberIpAddress() override;

    void setfRememberConnectPort(bool fRememberConnectPort) override;
    bool fRememberConnectPort() override;

    void            setAdbCommand(const QString& adbCommand) override;
    QString         adbCommand() override;

    void            setConnectInstructionText(const QString& instructionText) override;
    void            setPairInstructionText(const QString& instructionText) override;
    void            setProgress(int progress) override;

    void            clearLog();
    void            startProgress();

    QString         getAdbProcessCommand(const QString& applicationFilePath) override;
    void            startAction() override;
    void            finishAction() override;


private slots:
    void help();

private:

    void    showInformation();

    QString _adbCommand;
    QString connectInformationText, pairInformationText;

    Ui::MainDialog mainDialogUi;
    QDialog mainDialog;

    Ui::ActionDialog actionDialogUi;
    QDialog actionDialog;

    void            handleIpAddressChanged();
    void            handleConnectPortChanged();

    void            setForegroundToBackground(QWidget *widget);

    void            resizeButtonBoxButtons();
    void            doAction();
    void            stopAction();
    void            showConnect();
    void            showPair();
    void            showInstructions();

    QPushButton     *stopButton;
    QPushButton     *actionButton;

    bool            _isDoingAction = false;

    QString         _parsedIpAddress;
    QString         _parsedConnectPort;
    QString         _parsedPairPort;

    QSettings      _settings;
};


#endif

