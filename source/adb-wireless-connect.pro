# Add QT modules

QT       += core gui widgets

# Turn off console

CONFIG -= console

# Set compiler options

CONFIG += c++17
macx:CONFIG += app_bundle

# Set the platformm used by the build path

unix:PLATFORM = "Linux"
win32:PLATFORM = "Windows"
macx:PLATFORM = "macOS"


# Set path variables

ROOTDIR = ..
SOURCEDIR = .


# Set the build path

CONFIG(debug, debug|release) {
    DESTDIR = $$ROOTDIR/builds/$$PLATFORM/debug/adb-wireless-connect
}
CONFIG(release, debug|release) {
    DESTDIR = $$ROOTDIR/builds/$$PLATFORM/release/adb-wireless-connect
}

unix:TARGET = "adb-wireless-connect"
win32:TARGET = "ADBWirelessConnect"
macx:TARGET = "ADB Wireless Connect"

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui



# Set the application icon

linux:RC_ICONS = adb-wireless-connect.ico
win32:RC_ICONS = adb-wireless-connect.ico
macx:ICON = adb-wireless-connect.icns


# Resources

RESOURCES += $$SOURCEDIR/resources/adb-wireless-connect.qrc


# Main

SOURCES     +=  $$SOURCEDIR/main/main.cpp \



# View

HEADERS     +=  $$SOURCEDIR/views/view.h \
                $$SOURCEDIR/views/gui_view.h \
                $$SOURCEDIR/views/windows_console_fix.h \

SOURCES     +=  $$SOURCEDIR/views/view.cpp \
                $$SOURCEDIR/views/gui_view.cpp \
                $$SOURCEDIR/views/windows_console_fix.cpp \

FORMS       +=  $$SOURCEDIR/views/ui/main_dialog.ui \
                $$SOURCEDIR/views/ui/action_dialog.ui \


INCLUDEPATH +=  $$SOURCEDIR/views \


# Presenters

HEADERS     +=  $$SOURCEDIR/presenters/presenter.h \


SOURCES     +=  $$SOURCEDIR/presenters/presenter.cpp \


INCLUDEPATH +=  $$SOURCEDIR/presenters \


# Controllers

HEADERS     +=  $$SOURCEDIR/controllers/controller.h \


SOURCES     +=  $$SOURCEDIR/controllers/controller.cpp \


INCLUDEPATH +=  $$SOURCEDIR/controllers \

