#include "client.h"


JClient::JClient()
{

}

JClient::~JClient()
{

}

void JClient::connectToHost(QString hostAddr)
{
    qDebug() << "connect to host";

    m_port = 6666;
    m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket->connectToHost(hostAddr, m_port);

    JThread *thread = new JThread(this, this);
    thread->setSocket(m_tcpSocket);

    connect(thread, SIGNAL(newConnect(QString)), this, SIGNAL(newConnect(QString)));
    connect(thread, SIGNAL(byteArrived(QString,QString,QString)), this, SIGNAL(byteArrived(QString,QString,QString)));
    connect(thread, SIGNAL(disConnect(QString)), this, SLOT(disconnect(QString)));

    thread->start();
}

void JClient::deleteConnect(QString name)
{
    emit closeConnect(name);
}

void JClient::sendMessage(QString message, QString name)
{
    emit sendMessages(message, name);
}

void JClient::disconnect(QString name)
{
    emit finishConnect(name);
}
