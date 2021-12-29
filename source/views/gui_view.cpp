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

#include "gui_view.h"


GuiView::GuiView(QObject *parent)
    : View(parent)
{

}


GuiView::~GuiView()
{
    delete (presenter);
}

int GuiView::exec()
{
    if (!executable())
        return 1;

    return qApp->exec();
}

void GuiView::init(const QString& ipAddress, const QString& connectPort,
                   const QString& pairPort) {

    #ifdef Q_OS_LINUX
    QApplication::setWindowIcon(QIcon(":images/adb-wireless-connect.png"));
    #endif

     #ifdef Q_OS_WIN32
    QApplication::setWindowIcon(QIcon(":images/adb-wireless-connect.png"));
    #endif

     #ifdef Q_OS_MACX
     QApplication::setWindowIcon(QIcon(":images/adb-wireless-connect.icns"));
     #endif

    mainDialogUi.setupUi(&mainDialog);

    actionDialogUi.setupUi(&actionDialog);


    stopButton = new QPushButton(tr("&Stop"));
    stopButton->setAutoDefault(true);
    actionDialogUi.buttonBox->addButton(stopButton, QDialogButtonBox::ActionRole);

    mainDialog.show();
    presenter = new Presenter(this);


    actionButton = new QPushButton(tr("&Connect"));
    actionButton->setAutoDefault(true);
    actionButton->setDefault(true);
    mainDialogUi.buttonBox->addButton(actionButton, QDialogButtonBox::ActionRole);

    mainDialogUi.versionLabel->setText("Version " + QCoreApplication::applicationVersion());


    connect(actionButton, &QPushButton::clicked,
            this, &GuiView::doAction);

    connect(stopButton, &QPushButton::clicked,
            this, &GuiView::stopAction);

    connect(actionDialogUi.buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked,
            this, &GuiView::stopAction);

    connect(mainDialogUi.buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked,
            this, &GuiView::stopAction);

    connect(mainDialogUi.buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked,
            this, &GuiView::help);

    connect(mainDialogUi.connectRadioButton, &QRadioButton::clicked,
            this, &GuiView::showConnect);

    connect(mainDialogUi.pairRadioButton, &QRadioButton::clicked,
            this, &GuiView::showPair);

    connect(mainDialogUi.instructionPushButton, &QPushButton::clicked,
            this, &GuiView::showInstructions);

    connect(mainDialogUi.ipAddressLineEdit, &QLineEdit::textChanged,
            this, &GuiView::handleIpAddressChanged);


    connect(mainDialogUi.connectPortLineEdit, &QLineEdit::textChanged,
            this, &GuiView::handleConnectPortChanged);

    // put the pairPortLine Edit in the same place as connectPortLine Edit

    mainDialogUi.pairPortLineEdit->setGeometry(mainDialogUi.connectPortLineEdit->geometry());

    if (mainDialogUi.connectRadioButton->isChecked())
      showConnect();
    else if (mainDialogUi.pairRadioButton->isChecked())
      showPair();

    mainDialogUi.imageLabel->setPixmap(QPixmap(":images/adb-wireless-connect.png"));

    setForegroundToBackground(mainDialogUi.adbCommandLineEdit);
    setForegroundToBackground(actionDialogUi.logTextEdit);
    setForegroundToBackground(actionDialogUi.progressBar);

    actionButton->setEnabled(false);

    setIpAddress(ipAddress);
    setConnectPort(connectPort);
    setPairPort(pairPort);


    if (ipAddress.isEmpty()) {
        setIpAddress(_settings.value("rememberedData/ipAddress").toString());
        setfRememberIpAddress(!this->ipAddress().isEmpty());
    } else
        setfRememberIpAddress(false);


    if (connectPort.isEmpty()) {
        setConnectPort(_settings.value("rememberedData/connectPort").toString());
        setfRememberConnectPort(!this->connectPort().isEmpty());
    } else
        setfRememberConnectPort(false);

}
void GuiView::showError(const QString& msg)
{
    QMessageBox::warning(&mainDialog,"Error", msg);
}

void GuiView::showInformation(const QString& msg)
{
    QMessageBox::information(&mainDialog,tr("Information"), msg);
}



void GuiView::handleIpAddressChanged() {
    actionButton->setEnabled(!mainDialogUi.ipAddressLineEdit->text().isEmpty());


}
void GuiView::handleConnectPortChanged() {


}

void GuiView::setIpAddress(const QString& ipAddress) {
    mainDialogUi.ipAddressLineEdit->setText(ipAddress);
    actionButton->setEnabled(!ipAddress.isEmpty());

}

QString GuiView::ipAddress() {
     return mainDialogUi.ipAddressLineEdit->text();
}

void GuiView::setConnectPort(const QString& connectPort) {
    mainDialogUi.connectPortLineEdit->setText(connectPort);

}

QString GuiView::connectPort() {
     return mainDialogUi.connectPortLineEdit->text();
}

void GuiView::setPairPort(const QString& pairPort) {
    mainDialogUi.pairPortLineEdit->setText(pairPort);

}

QString GuiView::pairPort() {
     return mainDialogUi.pairPortLineEdit->text();
}

void GuiView::setPairCode(const QString& pairCode) {
    mainDialogUi.pairCodeLineEdit->setText(pairCode);

}

QString GuiView::pairCode() {
      return mainDialogUi.pairCodeLineEdit->text();
}

void GuiView::setParsedIpAddress(const QString& parsedIpAddress) {
     _parsedIpAddress = parsedIpAddress;
}

QString GuiView::parsedIpAddress() {
    return _parsedIpAddress;

}

void GuiView::setParsedConnectPort(const QString& parsedConnectPort){
    _parsedConnectPort = parsedConnectPort;
}

QString GuiView::parsedConnectPort() {
    return _parsedConnectPort;
}

void GuiView::setParsedPairPort(const QString& parsedPairPort){
 _parsedPairPort = parsedPairPort;
}

QString GuiView::parsedPairPort() {
    return _parsedPairPort;
}

void GuiView::setfRememberIpAddress(bool fRememberIpAddress) {
     mainDialogUi.rememberIpAddressCheckBox->setChecked(fRememberIpAddress);
}

bool GuiView::fRememberIpAddress() {
    return  mainDialogUi.rememberIpAddressCheckBox->isChecked();
}

void GuiView::setfRememberConnectPort(bool fRememberConnectPort) {
     mainDialogUi.rememberConnectPortCheckBox->setChecked(fRememberConnectPort);
}

bool GuiView::fRememberConnectPort() {
    return  mainDialogUi.rememberConnectPortCheckBox->isChecked();
}

void GuiView::setAdbCommand(const QString& adbCommand) {
    _adbCommand = adbCommand;
    QString adbCommandText = "";

    adbCommandText += QDir::toNativeSeparators(_adbCommand);
    // add the user-requested command arguments

    mainDialogUi.adbCommandLineEdit->setText(adbCommandText);

}


QString GuiView::adbCommand() { return _adbCommand; }

void GuiView::setConnectInstructionText(const QString& instructionText) {
    connectInformationText = instructionText;
}

void GuiView::setPairInstructionText(const QString& instructionText) {
    pairInformationText = instructionText;
}


void GuiView::appendLog(const QString& logText)
{
    actionDialogUi.logTextEdit->append(logText);

}



QString GuiView::getAdbProcessCommand(const QString& applicationFilePath)
{
    QString adbProcessCommand;

    QString msg = QString("") + \
        tr("The adb command executable file cannot be found. ADB Tools must be installed correctly.") + \
        tr("\n\nThis error won't occur if the adb command executable file is in the current PATH or the current folder.") + \
        tr(" Alternatively, the path to the executable can be specified using the command line option") + ":\n --adb-command ""<PATH>"". " + \
        tr("\n\nWould you like to choose the location of the command executable file now?");

    QMessageBox::StandardButton reply = QMessageBox::question(&mainDialog, tr("Error: adb command not found"), msg, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QFileDialog dialog(&dialog, tr("Select adb command executable file"));
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setLabelText(QFileDialog::Accept,tr("Select"));
        dialog.setFilter(QDir::Executable);

        bool isFinished = false;
        while  (not(isFinished)) {
            isFinished = true;
            if (dialog.exec())
                adbProcessCommand = dialog.selectedFiles()[0];

            if (adbProcessCommand == applicationFilePath) {
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

void GuiView::showConnect() {
    mainDialogUi.pairPortLineEdit->setVisible(false);
    mainDialogUi.pairCodeLineEdit->setEnabled(false);
    mainDialogUi.pairCodeLabel->setEnabled(false);
    mainDialogUi.pairPortLineEdit->setVisible(false);
    mainDialogUi.connectPortLineEdit->setVisible(true);
    mainDialogUi.instructionPushButton->setText(tr("Connect &Instructions"));
    actionButton->setText(tr("&Connect"));
    mainDialogUi.rememberConnectPortCheckBox->setEnabled(true);
    clearLog();
}

void GuiView::showPair() {
    mainDialogUi.connectPortLineEdit->setVisible(false);
    mainDialogUi.pairPortLineEdit->setVisible(true);
    mainDialogUi.pairCodeLineEdit->setEnabled(true);
    mainDialogUi.pairCodeLabel->setEnabled(true);
    mainDialogUi.instructionPushButton->setText(tr("Pair &Instructions"));
    actionButton->setText(tr("&Pair"));
    mainDialogUi.rememberConnectPortCheckBox->setEnabled(false);
    clearLog();
}



void GuiView::help() {

    QString appName = QCoreApplication::applicationName().replace(" ","-");
    QString link = "https://github.com/Jalopy-Tech/" + appName + "/blob/main/" + appName + ".md";
    QDesktopServices::openUrl(QUrl(link));
}

void GuiView::doAction() {
    clearLog();

    if (mainDialogUi.connectRadioButton->isChecked()) {
        actionDialog.setWindowTitle(tr("Connect"));
        emit adbConnectRequested();

    } else if (mainDialogUi.pairRadioButton->isChecked()) {
        actionDialog.setWindowTitle(tr("Pair"));
        emit adbPairRequested();
    }
}


void GuiView::startAction()
{
    actionDialog.show();
    stopButton->setEnabled(true);
    _isDoingAction = true;
}

void GuiView::finishAction()
{
    stopButton->setEnabled(false);
    _isDoingAction = false;
}

void GuiView::stopAction() {
    if (_isDoingAction)
        emit adbStopRequested();

}


void GuiView::setProgress(int progress)
{

    // update the progress based on incoming info

    actionDialogUi.progressBar->setValue(progress);
}



void GuiView::clearLog() {
    actionDialogUi.logTextEdit->clear();
    actionDialogUi.progressBar->setValue(0);
}



void GuiView::setForegroundToBackground(QWidget *widget) {
    QPalette pal = widget->palette();
    pal.setColor(QPalette::Active, QPalette::Base, widget->palette().color(QPalette::Active, QPalette::Window));
    widget->setPalette(pal);
}

void GuiView::showInstructions()
{

    if (mainDialogUi.connectRadioButton->isChecked())
      QMessageBox::information(&mainDialog,tr("Connect Instructions"), connectInformationText);
    else if (mainDialogUi.pairRadioButton->isChecked())
      QMessageBox::information(&mainDialog,tr("Pair Instructions"), pairInformationText);


}

