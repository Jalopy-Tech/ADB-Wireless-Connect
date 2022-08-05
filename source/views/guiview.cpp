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

#include "guiview.h"


GuiView::GuiView(QObject *parent)
    : View(parent)
{

}


GuiView::~GuiView()
{
    delete(presenter);
}

int GuiView::exec()
{
    if(!executable())
        return 1;

    return qApp->exec();
}

void GuiView::init(const QString& ipAddress,
                   const QString& connectPort,
                   const QString& pairPort,
                   const QString& adbCommand)
{

    // Set up application icon

    #ifdef Q_OS_LINUX
    QApplication::setWindowIcon(QIcon(":images/adb-wireless-connect.png"));
    #endif

    #ifdef Q_OS_WIN32
    QApplication::setWindowIcon(QIcon(":images/adb-wireless-connect.png"));
    #endif

    #ifdef Q_OS_MACX
    QApplication::setWindowIcon(QIcon(":images/adb-wireless-connect.icns"));
    #endif

    // Set up main Window

    mainWindowUi.setupUi(&mainWindow);
    mainWindowUi.versionLabel->setText("Version " + QCoreApplication::applicationVersion());

    connect(mainWindowUi.connectPushButton, &QPushButton::clicked,
            this, &GuiView::showConnectDialog);

    connect(mainWindowUi.actionConnect, &QAction::triggered,
            this, &GuiView::showConnectDialog);

    connect(mainWindowUi.pairPushButton, &QPushButton::clicked,
            this, &GuiView::showPairDialog);

    connect(mainWindowUi.actionPair, &QAction::triggered,
            this, &GuiView::showPairDialog);

    connect(mainWindowUi.disconnectPushButton, &QPushButton::clicked,
            this, &GuiView::doDisconnect);

    connect(mainWindowUi.actionDisconnect, &QAction::triggered,
            this, &GuiView::doDisconnect);

    connect(mainWindowUi.disconnectAllPushButton, &QPushButton::clicked,
            this, &GuiView::doDisconnectAll);

    connect(mainWindowUi.actionDisconnectAll, &QAction::triggered,
            this, &GuiView::doDisconnectAll);

    connect(mainWindowUi.refreshPushButton, &QPushButton::clicked,
            this, &GuiView::doList);

    connect(mainWindowUi.actionRefresh, &QAction::triggered,
            this, &GuiView::doList);

    connect(mainWindowUi.stopPushButton, &QPushButton::clicked,
            this, &GuiView::stopAction);

    connect(mainWindowUi.actionStop, &QAction::triggered,
            this, &GuiView::stopAction);

    connect(mainWindowUi.actionHelp, &QAction::triggered,
            this, &GuiView::help);

    connect(mainWindowUi.actionAbout, &QAction::triggered,
            this, &GuiView::showAbout);

    connect(mainWindowUi.actionExit, &QAction::triggered,
            this, &GuiView::exit);

    connect(mainWindowUi.clearLogPushButton, &QPushButton::clicked,
            this, &GuiView::clearLog);

    mainWindowUi.imageLabel->setPixmap(QPixmap(":images/adb-wireless-connect.png"));

    setForegroundToBackground(mainWindowUi.adbCommandLineEdit);
    setForegroundToBackground(mainWindowUi.logTextEdit);
    setForegroundToBackground(mainWindowUi.progressBar);

    connect(mainWindowUi.logToolButton, &QPushButton::clicked, this, &GuiView::toggleShowOrHideLog);
    connect(mainWindowUi.actionShowLog, &QAction::triggered, this, &GuiView::showLog);
    connect(mainWindowUi.actionHideLog, &QAction::triggered, this, &GuiView::hideLog);
    connect(mainWindowUi.actionClearLog, &QAction::triggered, this, &GuiView::clearLog);

    connect(mainWindowUi.connectedDevicesTable, &QTableWidget::itemSelectionChanged, this, &GuiView::changeDisconnectAbility);


    // Set up connect dialog

    connectDialogUi.setupUi(&connectDialog);

    connectDialogUi.buttonBox->button(QDialogButtonBox::Ok)->setText("&Connect");

    connect(connectDialogUi.buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &GuiView::doConnect);

    connect(connectDialogUi.instructionPushButton, &QPushButton::clicked,
            this, &GuiView::showConnectInstructions);

    connect(connectDialogUi.ipAddressLineEdit, &QLineEdit::textChanged,
            this, &GuiView::changeConnectAbility);



    // Set up pair dialog

    pairDialogUi.setupUi(&pairDialog);
    pairDialogUi.buttonBox->button(QDialogButtonBox::Ok)->setText("&Pair");

    connect(pairDialogUi.buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &GuiView::doPair);

    connect(pairDialogUi.instructionPushButton, &QPushButton::clicked,
            this, &GuiView::showPairInstructions);

    connect(pairDialogUi.ipAddressLineEdit, &QLineEdit::textChanged,
            this, &GuiView::changePairAbility);

    connect(pairDialogUi.pairCodeLineEdit, &QLineEdit::textChanged,
            this, &GuiView::changePairAbility);



    // #ifdef Q_OS_MACX
    // setGeometry(x(), y(), width(), height() + 75);
    //#endif

    _originalHeight = mainWindowUi.logToolButton->y() + 90;

    hideLog();

    QHeaderView* header =  mainWindowUi.connectedDevicesTable->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);

    _focusedPushButton = mainWindowUi.connectPushButton;

    mainWindow.show();

    presenter = new Presenter(this, ipAddress, connectPort, pairPort, adbCommand);
}


void GuiView::showError(const QString& msg)
{
    QMessageBox::warning(&mainWindow, "Error", msg);
}

void GuiView::showInformation(const QString& msg)
{
    QMessageBox::information(&mainWindow, tr("Information"), msg);
}

void GuiView::changeConnectAbility()
{
    bool fAllowed = !connectDialogUi.ipAddressLineEdit->text().isEmpty();
    connectDialogUi.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(fAllowed);
}

void GuiView::changePairAbility()
{
    bool fAllowed = !pairDialogUi.ipAddressLineEdit->text().isEmpty() && \
                    !pairDialogUi.pairCodeLineEdit->text().isEmpty();

    pairDialogUi.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(fAllowed);
}

void GuiView::changeDisconnectAbility()
{
    int row = mainWindowUi.connectedDevicesTable->currentRow();
    bool fAllowed = (row >= 0);

    mainWindowUi.disconnectPushButton->setEnabled(fAllowed);
    mainWindowUi.actionDisconnect->setEnabled(fAllowed);

}

QString GuiView::GuiView::connectedDevice() const
{
    int row = mainWindowUi.connectedDevicesTable->currentRow();

    if(row >= 0)
        return mainWindowUi.connectedDevicesTable->item(row, 0)->text();
    else
        return QString("");
}

void GuiView::setAdbCommand(const QString& adbCommand)
{
    _adbCommand = adbCommand;
    QString adbCommandText = "";

    adbCommandText += QDir::toNativeSeparators(_adbCommand);

    mainWindowUi.adbCommandLineEdit->setText(adbCommandText);
}

QString GuiView::adbCommand() const
{
    return _adbCommand;
}

void GuiView::setConnectInstructions(const QString& text)
{
    _connectInstructionsText = text;
}

void GuiView::setPairInstructions(const QString& text)
{
    _pairInstructionsText = text;
}

void GuiView::setAbout(const QString& text)
{
    _aboutText = text;
}

void GuiView::appendLog(const QString& logText)
{
    mainWindowUi.logTextEdit->append(logText);
}


QString GuiView::getAdbProcessCommand(const QString& applicationFilePath)
{
    QString adbProcessCommand;

    QString msg = QString("") + \
                  tr("The adb command executable file cannot be found. ADB Tools must be installed correctly.") + \
                  tr("\n\nThis error won't occur if the adb command executable file is in the current PATH or the current folder.") + \
                  tr(" Alternatively, the path to the executable can be specified using the command line option") + ":\n --adb-command ""<PATH>"". " + \
                  tr("\n\nWould you like to choose the location of the command executable file now?");

    QMessageBox::StandardButton reply = QMessageBox::question(&mainWindow, tr("Error: adb command not found"), msg, QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        QFileDialog dialog(&dialog, tr("Select adb command executable file"));
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setLabelText(QFileDialog::Accept, tr("Select"));
        dialog.setFilter(QDir::Executable);

        bool isFinished = false;
        while(not(isFinished)) {
            isFinished = true;
            if(dialog.exec())
                adbProcessCommand = dialog.selectedFiles()[0];

            if(adbProcessCommand == applicationFilePath) {
                isFinished = false;
                QString msg = QString("") + \
                              tr("The adb command executable file cannot be this application. Please choose again...");

                showError(msg);
                adbProcessCommand = "";
            }
        } // while

    } else
        adbProcessCommand = "";

    return adbProcessCommand;
}

void GuiView::clearConnectedDevices()
{
    mainWindowUi.connectedDevicesTable->setRowCount(0);
    changeDisconnectAbility();
}

void GuiView::showConnectDialog()
{
    connectDialogUi.ipAddressLineEdit->setText(_ipAddress);
    connectDialogUi.rememberIpAddressCheckBox->setChecked(_fRememberIpAddress);
    connectDialogUi.connectPortLineEdit->setText(_connectPort);
    connectDialogUi.rememberConnectPortCheckBox->setChecked(_fRememberConnectPort);
    changeConnectAbility();
    connectDialogUi.ipAddressLineEdit->setFocus();
    connectDialog.show();
}

void GuiView::showPairDialog()
{
    pairDialogUi.ipAddressLineEdit->setText(_ipAddress);
    pairDialogUi.rememberIpAddressCheckBox->setChecked(_fRememberIpAddress);
    pairDialogUi.pairPortLineEdit->setText(_pairPort);
    pairDialogUi.pairCodeLineEdit->setText(_pairCode);
    changePairAbility();
    pairDialogUi.ipAddressLineEdit->setFocus();
    pairDialog.show();
}


void GuiView::help()
{
    QString appName = QCoreApplication::applicationName().replace(" ", "-");
    QString link = "https://github.com/Jalopy-Tech/" + appName + "/blob/main/" + appName + ".md";
    QDesktopServices::openUrl(QUrl(appConstants::APPLICATION_WEBSITE));
}


void GuiView::exit()
{
    mainWindow.close();
}

void GuiView::doList()
{
    _focusedPushButton = mainWindowUi.refreshPushButton;

    emit adbListRequested();
}

void GuiView::doConnect()
{
    _focusedPushButton = mainWindowUi.connectPushButton;

    _ipAddress = connectDialogUi.ipAddressLineEdit->text();

    _fRememberIpAddress = connectDialogUi.rememberIpAddressCheckBox->isChecked();
    _connectPort = connectDialogUi.connectPortLineEdit->text();

    emit adbConnectRequested(connectDialogUi.ipAddressLineEdit->text(),
                             connectDialogUi.rememberIpAddressCheckBox->isChecked(),
                             connectDialogUi.connectPortLineEdit->text(),
                             connectDialogUi.rememberConnectPortCheckBox->isChecked()
                            );
}

void GuiView::doPair()
{
    _focusedPushButton = mainWindowUi.pairPushButton;

    _ipAddress = pairDialogUi.ipAddressLineEdit->text();
    _fRememberIpAddress = connectDialogUi.rememberIpAddressCheckBox->isChecked();
    emit adbPairRequested(pairDialogUi.ipAddressLineEdit->text(),
                          pairDialogUi.rememberIpAddressCheckBox->isChecked(),
                          pairDialogUi.pairPortLineEdit->text(),
                          pairDialogUi.pairCodeLineEdit->text());
}

void GuiView::doDisconnect()
{
    QString msg;
    _focusedPushButton = mainWindowUi.disconnectPushButton;

    int row = mainWindowUi.connectedDevicesTable->currentRow();

    if(row >= 0)
        emit adbDisconnectRequested(mainWindowUi.connectedDevicesTable->item(row, 0)->text());
    else {
        msg = QString("") + \
              tr("A connected device needs to be selected in order to disconnect it.\n");
        showError(msg);
    }
}

void GuiView::doDisconnectAll()
{
    _focusedPushButton = mainWindowUi.disconnectAllPushButton;
    emit adbDisconnectAllRequested();
}


void GuiView::disableActions()
{
    mainWindowUi.stopPushButton->setEnabled(true);
    mainWindowUi.actionStop->setEnabled(true);
    mainWindowUi.stopPushButton->setFocus();

    mainWindowUi.connectPushButton->setEnabled(false);
    mainWindowUi.actionConnect->setEnabled(false);
    mainWindowUi.pairPushButton->setEnabled(false);
    mainWindowUi.actionPair->setEnabled(false);
    mainWindowUi.disconnectPushButton->setEnabled(false);
    mainWindowUi.actionDisconnect->setEnabled(false);
    mainWindowUi.disconnectAllPushButton->setEnabled(false);
    mainWindowUi.actionDisconnectAll->setEnabled(false);
    mainWindowUi.refreshPushButton->setEnabled(false);
    mainWindowUi.actionRefresh->setEnabled(false);

    _isDoingAction = true;
}

void GuiView::enableActions()
{
    mainWindowUi.connectPushButton->setEnabled(true);
    mainWindowUi.actionConnect->setEnabled(true);
    mainWindowUi.pairPushButton->setEnabled(true);
    mainWindowUi.actionPair->setEnabled(true);
    changeDisconnectAbility();
    mainWindowUi.disconnectAllPushButton->setEnabled(true);
    mainWindowUi.actionDisconnectAll->setEnabled(true);
    mainWindowUi.refreshPushButton->setEnabled(true);
    mainWindowUi.actionRefresh->setEnabled(true);

    if(_focusedPushButton != NULL)
        _focusedPushButton->setFocus();

    _focusedPushButton = NULL;

    mainWindowUi.stopPushButton->setEnabled(false);
    mainWindowUi.actionStop->setEnabled(false);
    _isDoingAction = false;
}

void GuiView::stopAction()
{
    if(_isDoingAction)
        emit adbStopRequested();
}

void GuiView::setProgress(int progress)
{

    // update the progress based on incoming info

    mainWindowUi.progressBar->setValue(progress);
}

void GuiView::hideLog()
{
    // hide the log and set the window size to the orginal (non-expanded) size

    _isVisibleLog = false;
    mainWindow.resize(mainWindow.width(), _originalHeight);


    mainWindowUi.logTextEdit->hide();
    mainWindowUi.logToolButton->setArrowType(Qt::RightArrow);
    mainWindowUi.showHideLogLabel->setText("Show log");

    mainWindowUi.clearLogPushButton->setEnabled(false);
    mainWindowUi.actionClearLog->setEnabled(false);
    mainWindowUi.actionShowLog->setEnabled(true);
    mainWindowUi.actionHideLog->setEnabled(false);
}

void GuiView::showLog()
{
    // show the log and set the window size to an expanded size

    _isVisibleLog = true;
    int newHeight = _originalHeight + mainWindowUi.logTextEdit->height();
    if(newHeight > mainWindow.height())
        mainWindow.resize(mainWindow.width(), newHeight);


    mainWindowUi.logToolButton->setArrowType(Qt::DownArrow);
    mainWindowUi.showHideLogLabel->setText("Hide log");
    mainWindowUi.clearLogPushButton->setEnabled(true);
    mainWindowUi.actionClearLog->setEnabled(true);
    mainWindowUi.actionShowLog->setEnabled(false);
    mainWindowUi.actionHideLog->setEnabled(true);
    mainWindowUi.logTextEdit->show();
}

void GuiView::toggleShowOrHideLog()
{
    if(_isVisibleLog)
        hideLog();
    else
        showLog();
}

void GuiView::clearLog()
{
    mainWindowUi.logTextEdit->clear();
    mainWindowUi.progressBar->setValue(0);
}

void GuiView::setIpAddress(const QString& ipAddress)
{
    _ipAddress = ipAddress;
}

QString GuiView::ipAddress() const
{
    return _ipAddress;
}

void GuiView::setfRememberIpAddress(bool fRememberIpAddress)
{
    _fRememberIpAddress = fRememberIpAddress;
}

bool GuiView::fRememberIpAddress() const
{
    return  _fRememberIpAddress;
}

void GuiView::setConnectPort(const QString& connectPort)
{
    _connectPort = connectPort;
}

QString GuiView::connectPort() const
{
    return _connectPort;
}

void GuiView::setfRememberConnectPort(bool fRememberConnectPort)
{
    _fRememberConnectPort = fRememberConnectPort;
}

bool GuiView::GuiView::fRememberConnectPort() const
{
    return _fRememberConnectPort;
}

void GuiView::setPairPort(const QString& pairPort)
{
    _pairPort = pairPort;
}

QString GuiView::pairPort() const
{
    return _pairPort;
}

void GuiView::setPairCode(const QString& pairCode)
{
    _pairCode = pairCode;
}

QString GuiView::pairCode() const
{
    return _pairCode;
}


void GuiView::showConnectInstructions()
{
    QMessageBox::information(&connectDialog, tr("Connect Instructions"), _connectInstructionsText);
}

void GuiView::showPairInstructions()
{
    QMessageBox::information(&pairDialog, tr("Pair Instructions"), _pairInstructionsText);
}

void GuiView::showAbout()
{
    QMessageBox::information(&mainWindow, tr("About ") + QCoreApplication::applicationName(), _aboutText);
}

void GuiView::addConnectedDevice(const QString& device, const QString& qualifiers)
{
    QTableWidgetItem *newItem;

    int row = mainWindowUi.connectedDevicesTable->rowCount();
    mainWindowUi.connectedDevicesTable->insertRow(row);

    newItem = new QTableWidgetItem(device);
    mainWindowUi.connectedDevicesTable->setItem(row, 0, newItem);

    newItem = new QTableWidgetItem(qualifiers);
    mainWindowUi.connectedDevicesTable->setItem(row, 1, newItem);

    mainWindowUi.connectedDevicesTable->selectRow(0);
    //changeDisconnectAbility();
}

void GuiView::informListingDevicesReported(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
    QMessageBox::information(&mainWindow, tr("Listing devices"), message);
}

void GuiView::informDeviceConnected(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
}

void GuiView::informDeviceAlreadyConnected(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
    QMessageBox::information(&mainWindow, tr("Device already connected"), message);
}

void GuiView::informConnectingDeviceFailed(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
    QMessageBox::warning(&mainWindow, tr("Connecting to device failed"), message);
}

void GuiView::informConnectingDeviceReported(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
    QMessageBox::information(&mainWindow, tr("Connecting device"), message);
}

void GuiView::informDevicePaired(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
    QMessageBox::information(&mainWindow, tr("Device paired"), message);
}

void GuiView::informPairingDeviceFailed(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
    QMessageBox::warning(&mainWindow, tr("Pairing to device failed"), message);
}

void GuiView::informPairingDeviceReported(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
    QMessageBox::information(&mainWindow, tr("Pairing device"), message);
}

void GuiView::informDeviceDisconnected(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
}

void GuiView::informDisconnectingDeviceFailed(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
    QMessageBox::warning(&mainWindow, tr("Disconnecting device failed"), message);
}

void GuiView::informDisconnectingDeviceReported(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
    QMessageBox::information(&mainWindow, tr("Disconnecting device"), message);
}

void GuiView::informAllDevicesDisconnected(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
}

void GuiView::informDisconnectingAllDevicesReported(const QString& message)
{
    mainWindowUi.statusBar->showMessage(message);
    QMessageBox::information(&mainWindow, tr("Disconnecting all devices"), message);
}

void GuiView::setForegroundToBackground(QWidget *widget)
{
    QPalette pal = widget->palette();

    pal.setColor(QPalette::Active, QPalette::Base,
                 widget->palette().color(QPalette::Active, QPalette::Window));


    pal.setColor(QPalette::Inactive, QPalette::Base,
                 widget->palette().color(QPalette::Active, QPalette::Window));

    widget->setPalette(pal);
}
