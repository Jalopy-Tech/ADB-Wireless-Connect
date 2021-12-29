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
*/

#include "presenter.h"

Presenter::Presenter(View *view, QObject *parent)
    : QObject(parent)
{
    this->view = view;


    
    // setup coonnections with slots to handle running an external process

    connect(&_adbProcess, &QProcess::started, this, &Presenter::handleAdbStarted);
    connect(&_adbProcess, &QProcess::readyRead, this, &Presenter::handleAdbReadReady);
    connect(&_adbProcess, &QProcess::errorOccurred, this, &Presenter::handleAdbErrorOccurred);
    connect(&_adbProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), \
            [=](int exitCode, QProcess::ExitStatus exitStatus) { handleAdbFinished(exitCode, exitStatus); });


    // connect signals in preparation for the find art process

    connect(view, &View::adbConnectRequested, this, &Presenter::adbConnect);
    connect(view, &View::adbPairRequested, this, &Presenter::adbPair);
    connect(view, &View::adbStopRequested, this, &Presenter::adbStop);

    QString instructionText;

    instructionText = "";
    QFile connectTextFile(":text/connect-information.txt");
    if (connectTextFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

         QTextStream in(&connectTextFile);
         QString line = in.readLine();
         while (!line.isNull()) {
             instructionText += line;
             line = in.readLine();
         }
    }

    view->setConnectInstructionText(instructionText);


    instructionText = "";
    QFile pairTextFile(":text/pair-information.txt");
    if (pairTextFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

         QTextStream in(&pairTextFile);
         QString line = in.readLine();
         while (!line.isNull()) {
             instructionText += line;
             line = in.readLine();
         }
    }

    view->setPairInstructionText(instructionText);

}

void Presenter::adbConnect()
{

    if (view->fRememberIpAddress())
        _settings.setValue("rememberedData/ipAddress", view->ipAddress());
    else
        if (view->parsedIpAddress().isEmpty() ||view->ipAddress() != view->parsedIpAddress())
            _settings.setValue("rememberedData/ipAddress", "");

    if (view->fRememberConnectPort())
        _settings.setValue("rememberedData/connectPort", view->connectPort());
    else
        if (view->parsedConnectPort().isEmpty() || view->connectPort() != view->parsedConnectPort())
            _settings.setValue("rememberedData/connectPort", "");

    view->startAction();
    


    // Construct the external command line to find art.

    QString        adbProcessCommand;
    QStringList    adbProcessArguments;

    adbProcessCommand = QDir::toNativeSeparators(view->adbCommand());


    // add the standard album details arguments

    adbProcessArguments << "connect";

   if (!view->connectPort().isEmpty())
      adbProcessArguments  << view->ipAddress() + ":" + view->connectPort();
   else
     adbProcessArguments  << view->ipAddress();

    view->appendLog(tr("Connecting to:"));
    view->appendLog(tr("IP Address: ") + view->ipAddress());
    if (!view->connectPort().isEmpty()) {
        view->appendLog(tr("Connect Port: ") + view->connectPort());
    }

    view->appendLog("adb " + tr("command: ") + adbProcessCommand + " \"" +
                    adbProcessArguments.join("\" \"") + "\"");
    _progress = 10;



    // Set both stdout and stderr to the one output.

    _adbProcess.setProcessChannelMode(QProcess::MergedChannels);

    // Start the process. The rest will be covered by slots listening for signals from the process.

    _isProcessStopping = false;
    _adbProcess.start(adbProcessCommand, adbProcessArguments);

}

void Presenter::adbPair()
{


    if (view->fRememberIpAddress())
        _settings.setValue("rememberedData/ipAddress", view->ipAddress());
    else
        if (view->parsedIpAddress().isEmpty() || view->ipAddress() != view->parsedIpAddress())
            _settings.setValue("rememberedData/ipAddress", "");

    view->startAction();

    // Construct the external command line to find art.

    QString        adbProcessCommand;
    QStringList    adbProcessArguments;

    adbProcessCommand = QDir::toNativeSeparators(view->adbCommand());


    // add the standard album details arguments

    adbProcessArguments << "pair";

    if (!view->pairPort().isEmpty())
       adbProcessArguments  << view->ipAddress() + ":" + view->pairPort();
    else
      adbProcessArguments  << view->ipAddress();

    adbProcessArguments << view->pairCode();

    view->appendLog(tr("Pairing to:"));
    view->appendLog(tr("IP Address: ") + view->ipAddress());
    if (!view->pairPort().isEmpty()) {
        view->appendLog(tr("Pair Port: ") + view->pairPort());
    }
    if (!view->pairCode().isEmpty()) {
        view->appendLog(tr("Pair Code: ") + view->pairCode());
    }


     view->appendLog("adb " + tr("command: ") + adbProcessCommand + " \"" +
                     adbProcessArguments.join("\" \"") + "\"");
    _progress = 10;

    // Set both stdout and stderr to the one output.

    _adbProcess.setProcessChannelMode(QProcess::MergedChannels);

    // Start the process. The rest will be covered by slots listening for signals from the process.

    _adbProcess.start(adbProcessCommand, adbProcessArguments);

}

void Presenter::adbStop() {

    _isProcessStopping = true;
    _adbProcess.kill();
    _progress = 100;
    handleAdbProgressed(_progress, tr("Process stopped."));

    view->finishAction();

}

void Presenter::handleAdbStarted()
{


    handleAdbProgressed(_progress, tr("Process started."));
}

void Presenter::handleAdbReadReady()
{
    // Read the next line of outut from the process and sebd out a signal with the line data.

    char buf[1024];
    qint64 lineLength = _adbProcess.readLine(buf, sizeof(buf));
    if (lineLength != -1) {

        if (_progress < 80)
            _progress += 10;
        else if (_progress < 100)
            _progress += 1;

        QString progressText = buf;
        progressText = progressText.simplified();

        // Send out signals to any listeners (such as a controller) about the state of the process

        handleAdbProgressed(_progress, progressText);
    }
}

void Presenter::handleAdbErrorOccurred(QProcess::ProcessError error)
{
    QString msg = "";
    if (!_isProcessStopping) {

        if (error == QProcess::FailedToStart)
            msg = QString("") + \
                    tr("The adb command failed to start.");
        else  msg = QString("") + \
                tr("An error occurred when running the adb command.");

        msg = msg + tr("\n\nPlease check if ADB Tools have been installed properly.");
        view->showError(msg);

        view->finishAction();
    }
}

void Presenter::handleAdbFinished(int exitCode, QProcess::ExitStatus exitStatus)
{

    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        // Check for any final output before finishing up

        handleAdbReadReady();


        // Send out signals to any listeners (such as a controller) about the state of the process

        _progress = 100;


        handleAdbProgressed(_progress, tr("Process finished."));

    }


    view->finishAction();
}



void Presenter::handleAdbProgressed(int progress, const QString& progressText)
{

    view->setProgress(progress);
    view->appendLog(progressText);

}

