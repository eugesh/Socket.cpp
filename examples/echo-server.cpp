/* 
   echo-server.cpp

   Copyright (C) 2002-2017 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

#pragma comment(lib, "Ws2_32.lib")

#include "Socket.h"
// #include <process.h>
#include <string>
#include <iostream>
#include <QCoreApplication>

unsigned __stdcall Answer(void* a) {
    Socket* s = (Socket*) a;

    while (1) {
        int statusCode;
        std::string r = s->ReceiveLine(statusCode);
        if (r.empty())
            break;
        std::cout << "The line we received - " << r;
        s->SendLine(r, statusCode);
    }

    delete s;

    return 0;
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    int statusCode = 0;
    SocketServer in(2323, 1, statusCode);
    in.Start(statusCode);

    if (statusCode == 0)
    {
        while (1) {
            Socket* s = NULL;
            s = in.Accept(statusCode);

            if (s) {
                Answer(s);
                // unsigned ret;
                // _beginthreadex((void*)NULL, 0, Answer, (void*) s, 0, &ret);
                //break;
            }
        }
    } else {
        in.Close();
        printf("Something went wrong. The error code - %d.\n", statusCode);
    }

    return a.exec();

    /*WSADATA wsaData;
    int err = WSAStartup(0x202, &wsaData);

    if (err == 0)
    {
        int statusCode = 0;
        SocketServer in(2000, 1, "", statusCode);

        while (1) {
            Socket* s = NULL;
            s = in.Accept(statusCode);

            if (s) {
                unsigned ret;
                _beginthreadex((void*)NULL, 0, Answer, (void*) s, 0, &ret);
            }
        }
        WSACleanup();
    } else {
        printf("Something went wrong. The error code - %d.\n", err);
    }*/

    // return 0;
}
