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

#include "windows_console_fix.h"

namespace WindowsConsoleFix {

    void initialize() {

        // In Windows, redirect input, output and error to terminal window.
        // Unfortunately QT in Windows doesn't handle having a console
        // and GUI interface very well in one application. Linux and MacOS
        // don't have this issue. A more complete solution would be to have
        // two separate applications: one for console and one for GUI.

        #ifdef _WIN32

            int stdout_type = GetFileType(GetStdHandle(STD_OUTPUT_HANDLE));
            if(stdout_type == FILE_TYPE_UNKNOWN) {
                if (AttachConsole(ATTACH_PARENT_PROCESS)) {
                    HANDLE consoleHandleOut, consoleHandleError, consoleHandleIn;

                    // Redirect unbuffered STDOUT to the console
                    consoleHandleOut = GetStdHandle(STD_OUTPUT_HANDLE);
                    if (consoleHandleOut != INVALID_HANDLE_VALUE) {
                        freopen("CONOUT$", "w", stdout);
                        setvbuf(stdout, NULL, _IONBF, 0);
                    }

                    // Redirect unbuffered STDERR to the console
                    consoleHandleError = GetStdHandle(STD_ERROR_HANDLE);
                    if (consoleHandleError != INVALID_HANDLE_VALUE) {
                        freopen("CONOUT$", "w", stderr);
                        setvbuf(stderr, NULL, _IONBF, 0);
                    }

                    // Redirect unbuffered STDIN from the console
                    consoleHandleIn = GetStdHandle(STD_INPUT_HANDLE);
                    if (consoleHandleIn != INVALID_HANDLE_VALUE) {
                        freopen("CONIN$", "r", stdin);
                        setvbuf(stdin, NULL, _IONBF, 0);
                    }

                    printf ("\n");
                }
            }

        #endif


    }

    void finalize() {

        // In Windows, undo redirection (see comments above)
        // This includes sending an enter key

         #ifdef _WIN32

             // free the console from attachment

             FreeConsole();


             // send enter key to go back to command prompt

             INPUT keyboard;

             keyboard.type = INPUT_KEYBOARD;
             keyboard.ki.wScan = 0;
             keyboard.ki.time = 0;
             keyboard.ki.dwExtraInfo = 0;
             keyboard.ki.wVk = 0x0D;
             keyboard.ki.dwFlags = 0;

             SendInput(1, &keyboard, sizeof(INPUT));

             keyboard.ki.dwFlags = KEYEVENTF_KEYUP;
             SendInput(1, &keyboard, sizeof(INPUT));
          #endif

    }
}
