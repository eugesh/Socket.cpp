//#pragma once // comment(lib, "Ws2_32.lib")

#include "Socket.h"
#include <iostream>
#include <QCoreApplication>
#include <QDebug>
#include <QObject>

using namespace std;

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    try {
        int er = 0;
        SocketClient s("127.0.0.1", 2323, er); // "localhost"
        s.Start(er);

        string clientMessage;
        /*s.SendLine("Host: www.renenyffenegger.ch");
        s.SendLine("");*/

        while (1) {
            // getline(cin, clientMessage);
            clientMessage = "Hi!";
            int statusCode = 0;
            s.SendLine(clientMessage, statusCode);
            string l;
            do {
                string l = s.ReceiveLine(statusCode);
            } while (l.empty());
                // continue;
            cout << l;
            qInfo() << "The line we received - " << QString::fromStdString(l);
            cout.flush();
        }

    }
    catch (const char* s) {
        cerr << s << endl;
    }
    catch (std::string s) {
        cerr << s << endl;
    }
    catch (...) {
        cerr << "unhandled exception\n";
    }

    return a.exec();

    // return 0;
}
