/*send and receive message*/

#include "thread.h"

JThread::JThread(QObject *parent, JServer *ser):
    QThread(parent),
    ser(ser)
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
    connect(m_tcpsocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(ser, SIGNAL(sendMessages(QString,QString)), this, SLOT(sendMessage(QString,QString)));
    connect(ser, SIGNAL(closeConnect(QString)), this, SLOT(closeConnect(QString)));
    connect(m_tcpsocket, SIGNAL(disconnected()), this, SLOT(disConnect()));

    exec();//启动事件循环
}

void JThread::setSocket(QTcpSocket *socket)
{
    this->m_tcpsocket = socket;
}

void JThread::setConnectAddr(QString connectAddr)
{
    this->m_connectAddr = connectAddr;
}

void JThread::closeConnect(QString name)
{
    if(m_connectAddr == name)
    {
        m_tcpsocket->close();
    }
}

void JThread::disConnect()
{
    emit disConnect(m_connectAddr);
}

void JThread::receiveData()
{
    QDataStream in(m_tcpsocket);
    in.setVersion(QDataStream::Qt_4_6);

    QString message;
    if(m_tcpsocket->bytesAvailable() > 0)
    {
        in >> m_blockSize;
        in >> message;
        emit byteArrived(message, m_connectAddr);
    }
}

void JThread::sendMessage(QString message, QString name)
{
    if(message.isEmpty())
        return;

    if(m_connectAddr == name)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_6);
        out << (quint16)0;
        out << message;
        out.device()->seek(0);
        out << (quint16)(block.size()-sizeof(quint16));

        m_tcpsocket->write(block);
    }
}




