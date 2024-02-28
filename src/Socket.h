#pragma comment(lib, "Ws2_32.lib") // for sockets and tcp/ip connection
/* 
   Socket.h

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

#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <exception>

enum TypeSocket {BlockingSocket, NonBlockingSocket};

class SocketClientQt;
class SocketServerQt;

class Socket
{
public:

    virtual ~Socket();
    // Socket();
    Socket(const Socket&);
    Socket& operator=(Socket&);
    Socket(int &statusCode);

    virtual bool Start(int &statusCode) = 0; // int& statusCode

    virtual std::string ReceiveLine(int &statusCode) = 0;
    // virtual std::string ReceiveBytes() = 0;

    virtual void Close() = 0;

    // The parameter of SendLine is not a const reference
    // because SendLine modifes the std::string passed.
    virtual void SendLine(std::string &line, int &statusCode) = 0;

    // The parameter of SendBytes is a const reference
    // because SendBytes does not modify the std::string passed
    // (in contrast to SendLine).
    // virtual void SendBytes(const std::string&) = 0;

    // virtual void SendBytes(const char*, int) = 0;

protected:
    friend class SocketServer;

    // virtual bool Start(int &statusCode);
    // Socket(SOCKET s, int& statusCode);

    // SOCKET s_;

    // int* refCounter_;

private:
  // static void Start(int& statusCode);
  // static void End();
  // static int  nofSockets_;
};

class SocketClient : public Socket
{
public:
    SocketClient(const std::string &host, int port, int &statusCode);
    ~SocketClient() {}

    virtual bool Start(int &statusCode) override;

    virtual std::string ReceiveLine(int &statusCode) override;
    // virtual std::string ReceiveBytes() override;

    virtual void SendLine(std::string &line, int &statusCode) override;

    virtual void Close() override;

private:
    SocketClientQt    * m_qtClient = nullptr;
    // QByteArray         m_readData;
    // QList<QByteArray>  m_dataQ;
    // QString            m_hostAddr;
    // quint16            m_port;
};

class SocketServer : public Socket
{
public:
    SocketServer(int port, int connections, int &statusCode, TypeSocket type=BlockingSocket);
    virtual ~SocketServer() {}

    Socket *Accept(int &statusCode);
    virtual bool Start(int &statusCode) override;

    virtual std::string ReceiveLine(int& statusCode) override;
    // virtual std::string ReceiveBytes() override;

    virtual void SendLine(std::string &line, int &statusCode) override;

    virtual void Close() override;

private:
    // quint16             m_port;
    SocketServerQt       * m_qtServer;
    // QByteArray          m_readData;
};

/*class SocketSelect
{
// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/wsapiref_2tiq.asp
  public:
    SocketSelect(Socket const *const s1, Socket const *const s2 = NULL, TypeSocket type = BlockingSocket);

    bool Readable(Socket const *const s);

  private:
    fd_set fds_;
};*/


#endif
