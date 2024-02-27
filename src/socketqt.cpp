#include <QEventLoop>
#include <QTimer>

#include "socketqt.h"

#define AutoDisconnect(l) \
    QSharedPointer<QMetaObject::Connection> l = QSharedPointer<QMetaObject::Connection>(\
        new QMetaObject::Connection(), \
        [](QMetaObject::Connection * conn) { /*QSharedPointer сам по себе не производит отключения при удалении*/ \
            QObject::disconnect(*conn);\
        }\
    ); *l //-- Use AutoDisconnect(conn1) = connect(....);

SocketQt::SocketQt(QObject *parent)
    : QObject{parent}
{

}

SocketClientQt::SocketClientQt(const QString& hostAddr, int port, int& statusCode, QObject *parent)
 : Socket(statusCode), QObject(parent)
{
    // m_id = QUuid();
    m_socket    = new QTcpSocket(this);
    m_hostAddr  = hostAddr;
    m_port      = port;
    // m_connState = UnicanTCPclient::ConnNone;
    // m_srvRxState = UnicanTCPclient::RxIdle;
    // m_is_hardware = is_hardware;

    connect(m_socket, &QTcpSocket::connected,
            this, &SocketClientQt::slotConnected);
    connect(m_socket, &QTcpSocket::readyRead,
            this, &SocketClientQt::slotReadyRead);

#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, QOverload<QAbstractSocket::SocketError>::of(&SocketClientQt::slotError));
#else
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));
#endif
    connect(m_socket, &QTcpSocket::disconnected,
            this, &SocketClientQt::slotDisconnected);
    connect(m_socket, &QIODevice::bytesWritten,
            this, &SocketClientQt::handleBytesWritten);

    // changeStatus(UnicanTCPclient::ConnNone);
    // changeHwStatus(UnicanTCPclient::HwConnUnknown);
}

SocketClientQt::~SocketClientQt()
{
    if ((m_socket) && (m_socket->isOpen())) {
        m_socket->close();
        m_socket->deleteLater();
    }
    m_dataQ.clear();
    // m_sp_busy=false;
}

std::string SocketClientQt::ReceiveLine(int& statusCode)
{
    std::string data = m_readData.toStdString();

    m_readData.clear();

    return data;
}

void SocketClientQt::slotConnected()
{
    m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
}

void SocketClientQt::slotDisconnected()
{
    emit clientDisconnected();
    m_dataQ.clear();
    // m_sp_busy=false;
}

void SocketClientQt::slotReadyRead()
{
    m_readData.append(m_socket->readAll());
}

void SocketClientQt::start()
{
    m_dataQ.clear();
    m_readData.clear();
    // m_sp_busy=false;
    if (m_socket->state() != QAbstractSocket::ConnectingState)
        m_socket->connectToHost(m_hostAddr, m_port);
}

bool SocketClientQt::Start()
{
    start();

    return true;
}

void SocketClientQt::Close()
{
    slotDisconnected();
    m_socket->close();
}

void SocketClientQt::SendLine(std::string &str, int &statusCode)
{
    sendToServer(QByteArray::fromStdString(str));
}

void SocketClientQt::sendToServer(QByteArray const &arr, bool isNew)
{
    if (arr == nullptr)
        return;

    if (arr.isNull())
        return;

    if (isNew)
        m_dataQ.push_back(arr);

    // if (! m_sp_busy) {
        // m_sp_busy = true;
        qint64 bytesWritten = m_socket->write(arr);

        if (bytesWritten == -1)
        {
            qCritical() << QObject::tr("UnicanTCPclient: failed to write the data to host %1, error: %2")
                                .arg(m_hostAddr)
                                .arg(m_socket->errorString()) << endl;
        } else if (bytesWritten != arr.count()) {
            qCritical() << QObject::tr("UnicanTCPclient: failed to write all the data to host %1, error: %2")
                                .arg(m_hostAddr)
                                .arg(m_socket->errorString()) << endl;
        }
    // }
}

// void slotReconnect_toServer();

void SocketClientQt::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "TCP error: " + (err == QAbstractSocket::HostNotFoundError ?
                             "host not found" :
                             err == QAbstractSocket::RemoteHostClosedError ?
                             "remote host is closed" :
                             err == QAbstractSocket::ConnectionRefusedError ?
                             "connection refused" :
                             QString(m_socket->errorString()));

    // changeStatus(UnicanTCPclient::ConnError);
    // changeHwStatus(UnicanTCPclient::HwConnUnknown);

    if ((err == QAbstractSocket::HostNotFoundError) ||
        (err == QAbstractSocket::RemoteHostClosedError))
        qCritical() << strError;
}

void SocketClientQt::handleBytesWritten(qint64 bytes)
{
    if (m_dataQ.empty())
        return;

    if (bytes != m_dataQ.first().count())
        qCritical() << "Error: UnicanTCPclient::handleBytesWritten: length of data and written bytes isn't the same.";
    // m_sp_busy=false;
    m_dataQ.pop_front();

    if (m_dataQ.count())
        sendToServer(m_dataQ.first(), false);
}

SocketServerQt::SocketServerQt(quint16 port, int& statusCode, QObject *parent)
 : Socket(statusCode), QObject(parent)
{
    m_port = port;
    // m_srvRxState = UnicanTCPserver::RxIdle;
    m_tcpServer = new QTcpServer(this);
}

bool SocketServerQt::Start() // int& statusCode
{
    if (! m_tcpServer->listen(QHostAddress::Any, m_port))
    {
        qCritical() << "Unable to start the server: " << qPrintable(m_tcpServer->errorString());

        m_tcpServer->close();
        return false;
    }

    connect(m_tcpServer, &QTcpServer::newConnection,
            this, &SocketServerQt::slotNewConnection, Qt::UniqueConnection);

    return true;
}


/*
accept() {
    rerturn m_tcpServer->nextPendingConnection();
}*/

std::string SocketServerQt::ReceiveLine(int& statusCode)
{
    std::string data = m_readData.toStdString();

    m_readData.clear();

    return data;
}

/*std::string SocketServerQt::ReceiveBytes()
{

}*/

void SocketServerQt::slotReadClient()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());

    m_readData.append(clientSocket->readAll());
}

void SocketServerQt::slotDisconnected()
{

}

void SocketServerQt::slotCloseServer()
{

}

void
SocketServerQt::slotNewConnection()
{
    QTcpSocket * clientSocket = nullptr;
    clientSocket = m_tcpServer->nextPendingConnection();
    clientSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1); // LowDelayOption

    if (!clientSocket) {
        return;
    }

    connect(clientSocket, &QTcpSocket::disconnected,
            this, &SocketServerQt::slotDisconnected, Qt::UniqueConnection);
    connect(clientSocket, &QTcpSocket::readyRead,
            this, &SocketServerQt::slotReadClient, Qt::UniqueConnection);

    QEventLoop _loop;
    QTimer waitForConnectedTimer;
    AutoDisconnect(conn) = connect(&waitForConnectedTimer, &QTimer::timeout, [&_loop]() {
        // qDebug() << "Client connected!";
        _loop.exit();
    });

    waitForConnectedTimer.start(10000);

    _loop.exec();

    qInfo() << "New TCP connection";
}
