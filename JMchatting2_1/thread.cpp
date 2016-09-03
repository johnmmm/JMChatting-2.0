#include "thread.h"

JThread::JThread(QObject *parent, JClient *jh):
    QThread(parent),
    jh(jh)
{
    m_blockSize = 0;
}

JThread::~JThread()
{
    quit();
    deleteLater();
}

void JThread::run()
{
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(socketConnect()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnect()));
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(jh, SIGNAL(sendMessages(QString,QString)), this, SLOT(sendMessage(QString,QString)));
    connect(jh, SIGNAL(closeConnect(QString)), this, SLOT(closeConnect(QString)));

    exec();
}

void JThread::setSocket(QTcpSocket *socket)
{
    m_tcpSocket = socket;
}

void JThread::socketConnect()
{
    QString nameAddr(m_tcpSocket->localAddress().toString());
    nameAddr.append(":"+QString::number(m_tcpSocket->localPort()));
    m_localAddr = nameAddr;

    emit newConnect(m_localAddr);
}

void JThread::disconnect()
{
    emit disConnect(m_localAddr);
}

void JThread::closeConnect(QString name)
{
    if(m_localAddr == name)
    {
        m_tcpSocket->close();
    }
}

void JThread::receiveData()
{
    QDataStream in(m_tcpSocket);
    in.setVersion(QDataStream::Qt_4_6);
    QString connectAddr(m_tcpSocket->peerAddress().toString());
    connectAddr.append(":"+QString::number(m_tcpSocket->peerPort()));

    QString message;
    if(m_tcpSocket->bytesAvailable() > 0)
    {
        in >> m_blockSize;
        in >> message;
        emit byteArrived(message, connectAddr, m_localAddr);
    }
}

void JThread::sendMessage(QString message, QString name)
{
    if(message.isEmpty())
        return;

    if(m_localAddr == name)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_6);
        out << (quint16)0;
        out << message;
        out.device()->seek(0);
        out << (quint16)(block.size()-sizeof(quint16));

        m_tcpSocket->write(block);
    }
}

void JThread::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        QMessageBox::information(NULL, tr("Client"), tr("Disconnect with the server"),QMessageBox::Ok);
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(NULL, tr("Client"), tr("Cannot find the server, please check the IP!"),QMessageBox::Ok);
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(NULL, tr("Client"),
            tr("Connection rejected, please make sure that the server is on and check the IP!"), QMessageBox::Ok);
        break;
    default:
        QMessageBox::information(NULL, tr("Client"),
            tr("Wrong! %1.").arg(m_tcpSocket->errorString()),QMessageBox::Ok);
    }
}
