#pragma comment(lib, "IPHLPAPI.lib")
/* 
   Socket.cpp

   Copyright (C) 2002-2004 René Nyffenegger

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

#include "Socket.h"
#include "socketqt.h"
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

// int Socket::nofSockets_= 0;

/*void Socket::Start(int& statusCode) {
  if (!nofSockets_) {
    WSADATA info;
    if (WSAStartup(MAKEWORD(2,0), &info)) {
      statusCode = -1;
	  return;
    }
  }
  ++nofSockets_;
  statusCode = 0;
}

void Socket::End() {
  WSACleanup();
}*/

Socket::Socket(int& statusCode)
{
    Start(statusCode);
    if (statusCode != 0) {
        std::cerr << "Failed to start a socket.\n";
        return;
    }

    statusCode = 0;
}

bool Socket::Start(int& statusCode)
{

}

/*Socket::Socket(SOCKET s, int& statusCode) : s_(s) {
  Start(statusCode);
  if (statusCode != 0)
  {
	std::cerr << "Failed to start a socket.\n";
	return;
  }
  refCounter_ = new int(1);
  statusCode = 0;
};*/

Socket::~Socket() {
  /*if (! --(*refCounter_)) {
    Close();
    delete refCounter_;
  }

  --nofSockets_;
  if (!nofSockets_) End();*/
}

Socket::Socket(const Socket& o)
{
  /*refCounter_=o.refCounter_;
  (*refCounter_)++;
  s_         =o.s_;

  nofSockets_++;*/
}

/*Socket& Socket::operator=(Socket& o)
{
  (*o.refCounter_)++;

  refCounter_=o.refCounter_;
  s_         =o.s_;

  nofSockets_++;

  return *this;
}*/

void Socket::Close()
{
  //closesocket(s_);
}

bool SocketClient::Start(int& statusCode)
{
    statusCode = 0;

    return m_qtClient->Start();
}

std::string SocketClient::ReceiveLine(int& statusCode)
{
    return std::move(m_qtClient->ReceiveLine(statusCode));
}

/*std::string SocketClient::ReceiveBytes()
{
    return {};
}*/

bool SocketServer::Start(int& statusCode)
{
    statusCode = 0;

    return m_qtServer->Start();
}

/*if (! m_tcpServer->listen(QHostAddress::Any, m_port)) {
        qCritical() << "Unable to start the server: " << qPrintable(m_tcpServer->errorString());

        m_tcpServer->close();
        return false;
    }

    // connect(m_tcpServer, &QTcpServer::newConnection,
            // this, &UnicanTCPserver::slotNewConnection, Qt::UniqueConnection);

    return true;
}*/

std::string SocketServer::ReceiveLine(int& statusCode)
{
    return std::move(m_qtServer->ReceiveLine(statusCode));
}

/*std::string SocketServer::ReceiveBytes()
{
    return {};
}*/

SocketServer::SocketServer(int port, int connections, int& statusCode, TypeSocket type)
                            : Socket(statusCode)
{

}

/*Socket* SocketServer::Accept(int& statusCode)
{
    SOCKET new_sock = accept(s_, 0, 0);
    if (new_sock == INVALID_SOCKET) {
        int rc = WSAGetLastError();
        if(rc==WSAEWOULDBLOCK) {
            return 0; // non-blocking call, no request pending
        } else {
            statusCode = INVALID_SOCKET;
            std::cerr << "Failed to accept a socket.\n";
            return NULL;
        }
    }

    Socket* r = new Socket(new_sock, statusCode);
    if (statusCode != 0) {
        std::cerr << "Failed to initialize a socket.\n";
        return NULL;
    }

    statusCode = 0;
    return r;
}*/

SocketClient::SocketClient(const std::string& host, int port, int& statusCode) : Socket(statusCode)
{
    if (statusCode != 0)
        return; // Если произошла ошибка в конструкторе Socket

    m_qtClient = new SocketClientQt(QString::fromStdString(host), port, statusCode);

    statusCode = 0;
}

/* SocketClient::SocketClient(const std::string& host, int port, int& statusCode) : Socket(statusCode)
{
    if (statusCode != 0)	return;		// если произошла ошибка в конструкторе Socket
    char error[256];

    hostent *he;
    if ((he = gethostbyname(host.c_str())) == 0) {
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errno, MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
                    error, sizeof(error), NULL);
        statusCode = -1;
        std::cerr << error << "\n";
        return;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = *((in_addr *)he->h_addr);
    memset(&(addr.sin_zero), 0, 8);

    while (1)
    {
        int resultCode = ::connect(s_, (sockaddr *) &addr, sizeof(sockaddr));
        if (resultCode == SOCKET_ERROR)
        continue;
        break;
    }
    statusCode = 0;
}*/

/*SocketSelect::SocketSelect(Socket const * const s1, Socket const * const s2, TypeSocket type) {
  FD_ZERO(&fds_);
  FD_SET(const_cast<Socket*>(s1)->s_,&fds_);
  if(s2) {
    FD_SET(const_cast<Socket*>(s2)->s_,&fds_);
  }     

  TIMEVAL tval;
  tval.tv_sec  = 0;
  tval.tv_usec = 1;

  TIMEVAL *ptval;
  if(type==NonBlockingSocket) {
    ptval = &tval;
  }
  else { 
    ptval = 0;
  }

  if (select (0, &fds_, (fd_set*) 0, (fd_set*) 0, ptval) == SOCKET_ERROR) 
     throw "Error in select";
}

bool SocketSelect::Readable(Socket const* const s) {
  if (FD_ISSET(s->s_,&fds_)) return true;
     return false;
}*/

/*  if (statusCode != 0)	return;		// если произошла ошибка в конструкторе Socket

  sockaddr_in sa;

  memset(&sa, 0, sizeof(sa));

  sa.sin_family = PF_INET;
  sa.sin_port = htons(port);

  std::string ip_address;
  getAddressByAdapterName(adapater_name, ip_address, statusCode);
  if (statusCode != 0)
  {
    std::cerr << "Error: Failed to get the IP address in the specified network.\n";
    return;
  }

  sa.sin_addr.s_addr = inet_addr(ip_address.c_str());
  std::clog << "The address of host (in . notation): " << ip_address << "\n";

  s_ = socket(AF_INET, SOCK_STREAM, 0);
  if (s_ == INVALID_SOCKET) {
    std::cerr << "Error: INVALID SOCKET.\n";
    statusCode = INVALID_SOCKET;
    return;
  }

  if(type==NonBlockingSocket) {
    u_long arg = 1;
    ioctlsocket(s_, FIONBIO, &arg);
  }

  /* bind the socket to the internet address *//*
  if (bind(s_, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR) {
    std::cerr << "Error: INVALID SOCKET. The code of error - " << WSAGetLastError() << ".\n";
    closesocket(s_);
    WSACleanup();
    statusCode = SOCKET_ERROR;
    return;
  }

  std::clog << "Successfully binded.\n";
  statusCode = 0;
  listen(s_, connections);
}*/

/*void getAddressByAdapterName(const std::string& adapterName, std::string& ipAddress, int& statusCode)
{
    int result;

    // Get the length of all the adapters addresses
	ULONG buflen = 0;
	result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &buflen);
    if (result != ERROR_BUFFER_OVERFLOW)
	{
		std::cerr << "GetAdaptersAddresses failed with error: " << result << ".\n";
		statusCode = -1;
		return;
	}

	// Allocate memory for all the adapters addresses
	PIP_ADAPTER_ADDRESSES adaptersAddresses = (PIP_ADAPTER_ADDRESSES)malloc(buflen);
	if (!adaptersAddresses)
	{
		std::cerr << "Malloc failed.\n";
		statusCode = -1;
		return;
	}

	// Retreive all the adapters addresses (and store in adaptersAddresses)
	result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adaptersAddresses, &buflen);
    if (result != ERROR_SUCCESS)
	{
		std::cerr << "GetAdaptersAddresses failed with error: " << result << ".\n";
		free(adaptersAddresses);
		statusCode = -1;
		return;
	}

	PIP_ADAPTER_ADDRESSES adapter = NULL;
	std::wstring w_adapterName(adapterName.begin(), adapterName.end());

	for (PIP_ADAPTER_ADDRESSES adapterAddress = adaptersAddresses; adapterAddress != NULL; adapterAddress = adapterAddress->Next) 
	{
		if (w_adapterName.compare(adapterAddress->FriendlyName) == 0)
		{
			adapter = adapterAddress;
			break;
		}
	}

    // Get IPv4 address
    if (adapter)
	{
		PIP_ADAPTER_UNICAST_ADDRESS unicast;
        for (unicast = adapter->FirstUnicastAddress; unicast != NULL; unicast = unicast->Next)
		{
            if (unicast->Address.lpSockaddr->sa_family == AF_INET)					// if TCP network protocol is used
			{
				sockaddr_in* sockaddr = (sockaddr_in*)unicast->Address.lpSockaddr;
				char ipstr[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &(sockaddr->sin_addr), ipstr, INET_ADDRSTRLEN);
				ipAddress = ipstr;
				break;
			}
		}
	}
    else
	{
		std::cerr << "Couldn't find an adapter with the specified name - " << adapterName << " .\n";
		free(adaptersAddresses);
		statusCode = -1;
		return;
	}

	free(adaptersAddresses);
	statusCode = 0;
}*/

/*std::string Socket::ReceiveBytes() {
  std::string ret;
  char buf[1024];

  while (1) {
    u_long arg = 0;
    if (ioctlsocket(s_, FIONREAD, &arg) != 0)
      break;

    if (arg == 0)
      break;

    if (arg > 1024) arg = 1024;

    int rv = recv (s_, buf, arg, 0);
    if (rv <= 0) break;

    std::string t;

    t.assign (buf, rv);
    ret += t;
  }

  return ret;
}*/

/*std::string Socket::ReceiveLine(int& statusCode) {
  std::string ret;
  int status;
  while (1) {
    char r;
    status = recv(s_, &r, 1, 0);
    if (status == 0)
    {
        // not connected anymore;
        // ... but last line sent
        // might not end in \n,
        // so return ret anyway.
        return ret;
    }
    else if (status == SOCKET_ERROR)
    {
        statusCode = WSAGetLastError();
        switch (statusCode)
        {
            case WSAENETDOWN:
            {
                std::cerr << "Network is down. Socket operation encountered a dead network.\n";
                break;
            }
            case WSAENETRESET:
            {
                std::cerr << "Network dropped connection on reset.\n";
                break;
            }
            case WSAECONNABORTED:
            {
                std::cerr << "An established connection was aborted by the software in your host computer.\n";
                break;
            }
            case WSAECONNRESET:
            {
                std::cerr << "An existing connection was forcibly closed by the remote host.\n";
                break;
            }
            case WSAETIMEDOUT:
            {
                std::cerr << "Connection timed out. The connected host has failed to respond.\n";
                break;
            }
            default:
            {
                std::cerr << "Произошла неизвестная ошибка - " << statusCode << ".\n";
                break;
            }
        }		// switch clause
        return "";
    }

    statusCode = 0;
    ret += r;
    if (r == '\n')  return ret;
  }
}*/

/*void Socket::SendLine(std::string& s, int& statusCode) {
  s += '\n';
  int status;

  while (1) {
    status = send(s_,s.c_str(),s.length(),0);
    if (status != SOCKET_ERROR)	// everything went fine
    {
        statusCode = 0;
        return;
    }
    else 						// if error occured
    {
        statusCode = WSAGetLastError();
        switch (statusCode)
        {
            case WSAENETDOWN:
            {
                std::cerr << "Network is down. Socket operation encountered a dead network.\n";
                return;
            }
            case WSAENETRESET:
            {
                std::cerr << "Network dropped connection on reset.\n";
                return;
            }
            case WSAECONNABORTED:
            {
                std::cerr << "An established connection was aborted by the software in your host computer.\n";
                return;
            }
            case WSAECONNRESET:
            {
                std::cerr << "An existing connection was forcibly closed by the remote host.\n";
                return;
            }
            case WSAEHOSTUNREACH:
            {
                std::cerr << "Unreachable host.\n";
                return;
            }
            case WSAETIMEDOUT:
            {
                std::cerr << "Connection timed out. The connected host has failed to respond.\n";
                return;
            }
            default:
            {
                std::cerr << "Произошла неизвестная ошибка - " << statusCode << ".\n";
                return;
            }
        }		// switch clause
    }		// else if (status == SOCKET_ERROR)
  }	// while (1)
}*/

/*void Socket::SendBytes(const std::string& s) {
  send(s_,s.c_str(),s.length(),0);
}*/

// добавлено
/*void Socket::SendBytes(const char* buf, int len) {
  send(s_, buf, len, 0);
}*/

// void getAddressByAdapterName(const std::string& adapterName, std::string& ipAddress, int& statusCode);
