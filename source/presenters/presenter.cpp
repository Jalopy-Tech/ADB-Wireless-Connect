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
*/

#include "presenter.h"

Presenter::Presenter(View *view, const QString& ipAddress,
                     const QString& connectPort,
                     const QString& pairPort,
                     const QString& adbCommand,
                     QObject *parent)
    : QObject(parent)
{
    // Set the view

    this->view = view;


    // Set view values

    _ipAddress = ipAddress;
    _connectPort = connectPort;
    _pairPort = pairPort;
    _pairCode = "";

    _adbCommand = adbCommand;
    _nativeAdbProcessCommand = QDir::toNativeSeparators(_adbCommand);

    _parsedIpAddress = _ipAddress;
    _parsedConnectPort = _connectPort;
    _parsedPairPort = _pairPort;


    // Retrieve saved settings

    if(_ipAddress.isEmpty()) {
        _ipAddress = _settings.value("rememberedData/ipAddress").toString();
        _fRememberIpAddress = !(_ipAddress.isEmpty());
    } else
        _fRememberIpAddress = false;

    if(_connectPort.isEmpty()) {
        _connectPort = _settings.value("rememberedData/connectPort").toString();
        _fRememberConnectPort = !(_connectPort.isEmpty());
    } else
        _fRememberConnectPort = false;


    // Set initial view values

    view->setIpAddress(_ipAddress);
    view->setfRememberIpAddress(_fRememberIpAddress);
    view->setConnectPort(_connectPort);
    view->setfRememberConnectPort(_fRememberConnectPort);
    view->setPairPort(_pairPort);
    view->setPairCode(_pairCode);

    view->setAdbCommand(_adbCommand);


    // Set connect instructions for view

    _connectInstructionsText = "";
    QFile connectTextFile(":text/connect-information.txt");
    if(connectTextFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QTextStream in (&connectTextFile);
        QString line = in.readLine();
        while(!line.isNull()) {
            _connectInstructionsText += line;
            line = in.readLine();
        }
        connectTextFile.close();
    }
    _connectInstructionsText = replaceAppConstantTags(_connectInstructionsText);
    view->setConnectInstructions(_connectInstructionsText);


    // Set pair instructions for view

    _pairInstructionsText = "";
    QFile pairTextFile(":text/pair-information.txt");
    if(pairTextFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QTextStream in (&pairTextFile);
        QString line = in.readLine();
        while(!line.isNull()) {
            _pairInstructionsText += line;
            line = in.readLine();
        }
        pairTextFile.close();
    }
    _pairInstructionsText = replaceAppConstantTags(_pairInstructionsText);
    view->setPairInstructions(_pairInstructionsText);

    // Set "about" text for view

    _aboutText = "";
    //_aboutText += QString(appConstants::APPLICATION_NAME) + " ";
    //_aboutText += QString(appConstants::APPLICATION_VERSION);

    QFile aboutTextFile(":text/about.txt");
    if(aboutTextFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QTextStream in (&aboutTextFile);
        QString line = in.readLine();
        while(!line.isNull()) {
            _aboutText += line;
            line = in.readLine();
        }
        aboutTextFile.close();
    }

    _aboutText = replaceAppConstantTags(_aboutText);
    view->setAbout(_aboutText);


    // Setup coonnections with slots to handle running an external process

    connect(&_adbProcess, &QProcess::started, this, &Presenter::handleAdbStarted);
    connect(&_adbProcess, &QProcess::readyRead, this, &Presenter::handleAdbReadReady);
    connect(&_adbProcess, &QProcess::errorOccurred, this, &Presenter::handleAdbErrorOccurred);
    connect(&_adbProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), \
    [ = ](int exitCode, QProcess::ExitStatus exitStatus) {
        handleAdbFinished(exitCode, exitStatus);
    });


    // Connect signals in preparation for command processes

    connect(view, &View::adbListRequested, this, &Presenter::adbList);
    connect(view, &View::adbConnectRequested, this, &Presenter::adbConnect);
    connect(view, &View::adbPairRequested, this, &Presenter::adbPair);
    connect(view, &View::adbDisconnectRequested, this, &Presenter::adbDisconnect);
    connect(view, &View::adbDisconnectAllRequested, this, &Presenter::adbDisconnectAll);
    connect(view, &View::adbStopRequested, this, &Presenter::adbStop);


    // Force view to initialize the list of devices

    adbList();
}

Presenter::~Presenter()
{
    if(interpreter != NULL)
        delete interpreter;
}


void Presenter::adbList()
{
    view->disableActions();
    view->clearConnectedDevices();

    // Construct the external command

    QStringList    adbProcessArguments;
    adbProcessArguments << "devices";
    adbProcessArguments  << "-l";

    view->appendLog(tr("Listing devices:"));
    view->appendLog("adb " + tr("command: ") + _nativeAdbProcessCommand + " \"" +
                    adbProcessArguments.join("\" \"") + "\"");
    _progress = 10;


    // Set both stdout and stderr to the one output.

    _adbProcess.setProcessChannelMode(QProcess::MergedChannels);


    _isProcessStopping = false;


    // Set up the command interpreter to listen for lines,
    // interpret them, and emit signals

    ListInterpreter *listInterpreter = new ListInterpreter();
    connect(listInterpreter, &ListInterpreter::deviceListed, view, &View::addConnectedDevice);
    connect(listInterpreter, &ListInterpreter::listingDevicesReported, view, &View::informListingDevicesReported);

    interpreter = listInterpreter;


    // Start the process. The rest will be covered by slots listening
    // for signals from the process.

    _adbProcess.start(_nativeAdbProcessCommand, adbProcessArguments);

}

void Presenter::adbConnect(const QString& ipAddress,
                           bool fRememberIpAddress,
                           const QString& connectPort,
                           bool fRememberConnectPort)
{
    QString msg;

    if(ipAddress.isEmpty()) {
        msg = QString("") + \
              tr("The IP address is required to connect to a device.\n");

        view->showError(msg);
        view->enableActions();
        return;
    }

    if(interpreter == NULL)
        view->disableActions();

    _ipAddress = ipAddress;
    _fRememberIpAddress = fRememberIpAddress;
    _connectPort = connectPort;
    _fRememberConnectPort = fRememberConnectPort;

    if(_fRememberIpAddress)
        _settings.setValue("rememberedData/ipAddress", _ipAddress);
    else if(_parsedIpAddress.isEmpty() || _ipAddress != _parsedIpAddress)
        _settings.setValue("rememberedData/ipAddress", "");

    if(_fRememberConnectPort)
        _settings.setValue("rememberedData/connectPort", _connectPort);
    else if(_parsedConnectPort.isEmpty() || _connectPort != _parsedConnectPort)
        _settings.setValue("rememberedData/connectPort", "");

    view->setIpAddress(_ipAddress);
    view->setfRememberIpAddress(_fRememberIpAddress);
    view->setConnectPort(_connectPort);
    view->setfRememberConnectPort(_fRememberConnectPort);

    QStringList    adbProcessArguments;

    adbProcessArguments << "connect";

    if(!_connectPort.isEmpty())
        adbProcessArguments  << _ipAddress + ":" + _connectPort;
    else
        adbProcessArguments  << _ipAddress;

    view->appendLog(tr("Connecting to:"));
    view->appendLog(tr("IP Address: ") + _ipAddress);
    if(!view->connectPort().isEmpty())
        view->appendLog(tr("Connect Port: ") + _connectPort);


    view->appendLog("adb " + tr("command: ") + _nativeAdbProcessCommand + " \"" +
                    adbProcessArguments.join("\" \"") + "\"");
    _progress = 10;


    // Set both stdout and stderr to the one output.

    _adbProcess.setProcessChannelMode(QProcess::MergedChannels);


    _isProcessStopping = false;


    // Set up the command interpreter to listen for lines,
    // interpret them, and emit signals

    ConnectInterpreter *connectInterpreter = new ConnectInterpreter();
    connect(connectInterpreter, &ConnectInterpreter::deviceConnected, view, &View::informDeviceConnected);
    connect(connectInterpreter, &ConnectInterpreter::deviceAlreadyConnected, view, &View::informDeviceAlreadyConnected);
    connect(connectInterpreter, &ConnectInterpreter::connectingDeviceFailed, view, &View::informConnectingDeviceFailed);
    connect(connectInterpreter, &ConnectInterpreter::connectingDeviceReported, view, &View::informConnectingDeviceReported);
    interpreter = connectInterpreter;


    // Start the process. The rest will be covered by slots listening
    // for signals from the process.

    _adbProcess.start(_nativeAdbProcessCommand, adbProcessArguments);
}

void Presenter::adbPair(const QString& ipAddress,
                        bool fRememberIpAddress,
                        const QString& pairPort,
                        const QString& pairCode)
{
    QString msg;

    if(ipAddress.isEmpty() || pairCode.isEmpty()) {
        msg = QString("");
        if(ipAddress.isEmpty())
            msg += tr("The IP address is required to pair to a device.\n");
        if(pairCode.isEmpty())
            msg += tr("The pair code is required to pair to a device.\n");

        view->showError(msg);
        view->enableActions();
        return;
    }

    if(interpreter == NULL)
        view->disableActions();

    _ipAddress = ipAddress;
    _fRememberIpAddress = fRememberIpAddress;
    _pairPort = pairPort;
    _pairCode = pairCode;


    if(_fRememberIpAddress)
        _settings.setValue("rememberedData/ipAddress", _ipAddress);
    else if(_parsedIpAddress.isEmpty() || _ipAddress != _parsedIpAddress)
        _settings.setValue("rememberedData/ipAddress", "");

    view->setIpAddress(_ipAddress);
    view->setfRememberIpAddress(_fRememberIpAddress);
    view->setPairPort(_pairPort);
    view->setPairCode(_pairCode);

    QStringList    adbProcessArguments;

    // add the standard album details arguments

    adbProcessArguments << "pair";

    if(!_pairPort.isEmpty())
        adbProcessArguments  << _ipAddress + ":" + _pairPort;
    else
        adbProcessArguments  << _ipAddress;

    adbProcessArguments << _pairCode;

    view->appendLog(tr("Pairing to:"));
    view->appendLog(tr("IP Address: ") + _ipAddress);
    if(!_pairPort.isEmpty())
        view->appendLog(tr("Pair Port: ") + _pairPort);

    if(!pairCode.isEmpty())
        view->appendLog(tr("Pair Code: ") + _pairCode);

    view->appendLog("adb " + tr("command: ") + _nativeAdbProcessCommand + " \"" +
                    adbProcessArguments.join("\" \"") + "\"");
    _progress = 10;

    // Set both stdout and stderr to the one output.

    _adbProcess.setProcessChannelMode(QProcess::MergedChannels);

    _isProcessStopping = false;


    // Set up the command interpreter to listen for lines,
    // interpret them, and emit signals

    PairInterpreter *pairInterpreter = new PairInterpreter();
    connect(pairInterpreter, &PairInterpreter::devicePaired, view, &View::informDevicePaired);
    connect(pairInterpreter, &PairInterpreter::pairingDeviceFailed, view, &View::informPairingDeviceFailed);
    connect(pairInterpreter, &PairInterpreter::pairingDeviceReported, view, &View::informPairingDeviceReported);
    interpreter = pairInterpreter;

    // Start the process. The rest will be covered by slots listening
    // for signals from the process.

    _adbProcess.start(_nativeAdbProcessCommand, adbProcessArguments);
}

void Presenter::adbDisconnect(const QString& device)
{
    QString msg;

    if(device.isEmpty()) {
        msg = QString("") + \
              tr("A connected device needs to be selected in order to disconnect it.\n");

        view->showError(msg);
        view->enableActions();
        return;
    }

    if(interpreter == NULL)
        view->disableActions();

    // Construct the external command line.

    QStringList adbProcessArguments;


    // add the standard album details arguments

    adbProcessArguments << "disconnect";
    adbProcessArguments  << view->connectedDevice();

    view->appendLog(tr("Disconnecting from:"));
    view->appendLog(tr("Device: ") + device);

    if(device.isEmpty()) {

        msg = QString("") + \
              tr("Disconnecting from device failed because no device was specified.");
        view->showError(msg);

        adbList();
        return;
    }

    view->appendLog("adb " + tr("command: ") + _nativeAdbProcessCommand + " \"" +
                    adbProcessArguments.join("\" \"") + "\"");
    _progress = 10;


    // Set both stdout and stderr to the one output.

    _adbProcess.setProcessChannelMode(QProcess::MergedChannels);

    _isProcessStopping = false;


    // Start the process. The rest will be covered by slots listening
    // for signals from the process

    DisconnectInterpreter *disconnectInterpreter = new DisconnectInterpreter();
    connect(disconnectInterpreter, &DisconnectInterpreter::deviceDisconnected, view, &View::informDeviceDisconnected);
    connect(disconnectInterpreter, &DisconnectInterpreter::disconnectingDeviceFailed, view, &View::informDisconnectingDeviceFailed);
    connect(disconnectInterpreter, &DisconnectInterpreter::disconnectingDeviceReported, view, &View::informDisconnectingDeviceReported);
    interpreter = disconnectInterpreter;


    // Start the process. The rest will be covered by slots listening
    // for signals from the process.

    _adbProcess.start(_nativeAdbProcessCommand, adbProcessArguments);
}

void Presenter::adbDisconnectAll()
{
    if(interpreter == NULL)
        view->disableActions();

    // Construct the external command line.

    QStringList adbProcessArguments;

    // add the standard album details arguments

    adbProcessArguments << "disconnect";

    view->appendLog(tr("Disconnecting from all devices:"));
    view->appendLog("adb " + tr("command: ") + _nativeAdbProcessCommand + " \"" +
                    adbProcessArguments.join("\" \"") + "\"");
    _progress = 10;

    // Set both stdout and stderr to the one output.

    _adbProcess.setProcessChannelMode(QProcess::MergedChannels);

    _isProcessStopping = false;


    // Start the process. The rest will be covered by slots listening
    // for signals from the process

    DisconnectAllInterpreter *disconnectAllInterpreter = new DisconnectAllInterpreter();
    connect(disconnectAllInterpreter, &DisconnectAllInterpreter::allDevicesDisconnected, view, &View::informAllDevicesDisconnected);
    connect(disconnectAllInterpreter, &DisconnectAllInterpreter::disconnectingAllDevicesReported, view, &View::informDisconnectingAllDevicesReported);
    interpreter = disconnectAllInterpreter;

    // Start the process. The rest will be covered by slots listening
    // for signals from the process.

    _adbProcess.start(_nativeAdbProcessCommand, adbProcessArguments);
}

void Presenter::adbStop()
{
    _isProcessStopping = true;
    _adbProcess.kill();
    _progress = 100;
    handleAdbProgressed(_progress, interpreter->commandTitle() +  tr(" stopped."));
}


void Presenter::handleAdbStarted()
{
    handleAdbProgressed(_progress, interpreter->commandTitle() + tr(" started."));
}

void Presenter::handleAdbReadReady()
{
    // Read the next line of output from the process and sebd out a signal with the line data.

    char buf[1024];

    qint64 lineLength = _adbProcess.readLine(buf, sizeof(buf));
    if(lineLength != -1) {

        if(_progress < 80)
            _progress += 10;
        else if(_progress < 100)
            _progress += 1;

        QString progressText = buf;
        progressText = progressText.simplified();

        // Send out signals to any listeners (such as a controller) about the state of the process

        handleAdbProgressed(_progress, progressText);
        if(interpreter != NULL) {
            interpreter->interpret(progressText);
        }
    }
}

void Presenter::handleAdbErrorOccurred(const QProcess::ProcessError& error)
{
    QString msg = "";

    // No need to handle errors due to stopping the process

    if(!_isProcessStopping) {

        if(error == QProcess::FailedToStart)
            msg = QString("") + \
                  tr("The adb command failed to start.");
        else  msg = QString("") + \
                        tr("An error occurred while running the adb command.");

        msg = msg + tr("\n\nPlease check if ADB Tools have been installed properly.");
        view->showError(msg);

        view->enableActions();
    }
}

void Presenter::handleAdbFinished(int exitCode, const QProcess::ExitStatus& exitStatus)
{
    if(exitStatus == QProcess::NormalExit && exitCode == 0) {
        // Check for any final output before finishing up

        while(_adbProcess.canReadLine())
            handleAdbReadReady();

        // Send out signals to any listeners (such as a controller) about the state of the process

        _progress = 100;

        handleAdbProgressed(_progress, interpreter->commandTitle() + tr(" finished.\n"));
    }

    // Delete the command interpeter and force a "list devices"
    // to refresh devices in the view

    if(interpreter != NULL) {

        if(interpreter->commandType() == Interpreter::LIST_COMMAND) {
            delete interpreter;
            interpreter = NULL;
            view->enableActions();
        } else {
            delete interpreter;
            interpreter = NULL;
            adbList();
        }
    }
}

void Presenter::handleAdbProgressed(int progress, const QString& progressText)
{
    view->setProgress(progress);
    view->appendLog(progressText);
}

QString Presenter::replaceAppConstantTags(const QString& s) const
{
    QString r = s;

    r = r.replace("<APPLICATION_NAME>", appConstants::APPLICATION_NAME);
    r = r.replace("<APPLICATION_VERSION>", appConstants::APPLICATION_VERSION);
    r = r.replace("<APPLICATION_WEBSITE>", appConstants::APPLICATION_WEBSITE);
    r = r.replace("<ORGANIZATION_NAME>", appConstants::ORGANIZATION_NAME);
    r = r.replace("<COPYRIGHT_NAME>", appConstants::COPYRIGHT_NAME);
    r = r.replace("<COPYRIGHT_YEAR>", appConstants::COPYRIGHT_YEAR);

    return r;
}
