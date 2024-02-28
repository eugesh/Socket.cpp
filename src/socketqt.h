#ifndef SOCKETQT_H
#define SOCKETQT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "Socket.h"

/*class SocketQt : public QObject
{
    Q_OBJECT
public:
    explicit SocketQt(QObject *parent=nullptr);
    virtual QString ReceiveLine(int &statusCode) = 0;
    // virtual std::string ReceiveBytes();
    virtual bool Start() = 0;

signals:

};*/

class SocketClientQt : public QObject, public Socket
{
    Q_OBJECT
public:
    explicit SocketClientQt(const QString &host, int port, int &statusCode, QObject *parent=nullptr);
    explicit SocketClientQt(QTcpSocket *socket, int& statusCode);
    virtual ~SocketClientQt();

    virtual bool Start(int &statusCode) override; // int& statusCode

    virtual void Close() override;

    virtual void SendLine(std::string &str, int &statusCode) override;

    virtual std::string ReceiveLine(int &statusCode) override;
    // virtual std::string ReceiveBytes() override;

public slots:
    void slotConnected();
    void slotDisconnected();
    void slotReadyRead();
    void start();
    void sendToServer(QByteArray const &arr, bool isNew=true);
    // void slotReconnect_toServer();
    void slotError(QAbstractSocket::SocketError err);
    void handleBytesWritten(qint64 bytes);

signals:
    void clientDisconnected();

private:
    void initQt();

private:
    QTcpSocket       * m_socket = nullptr;
    QByteArray         m_readData;
    QList<QByteArray>  m_dataQ;
    QString            m_hostAddr;
    quint16            m_port;
};

class SocketServerQt : public QObject, public Socket
{
    Q_OBJECT
public:
    explicit SocketServerQt(quint16 port, int &statusCode, QObject *parent=nullptr);
    virtual ~SocketServerQt();

    virtual bool Start(int &statusCode) override; // int& statusCode

    virtual void Close() override;

    virtual void SendLine(std::string &line, int &statusCode) override;

    virtual std::string ReceiveLine(int &statusCode) override;
    // virtual std::string ReceiveBytes() override;

    Socket* Accept(int& statusCode);

signals:
    // void newClientAdded(ucd_client_info_t newcl);
    void signalDisconnected(quint32 clientId);

public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void slotDisconnected();
    void slotCloseServer();

private:
    // QUuid           m_id;
    quint16             m_port;
    QTcpServer        * m_tcpServer = nullptr;
    QList<QTcpSocket*>  m_clientSocketList;
    QByteArray          m_readData;
};

#endif // SOCKETQT_H
