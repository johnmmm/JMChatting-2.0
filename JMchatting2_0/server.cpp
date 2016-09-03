/*connection and let out some signals*/

#include "server.h"


JServer::JServer(QObject *parent):
    QTcpServer(parent)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(comingConnectionSlot()));
}


JServer::~JServer()
{

}


void JServer::deleteConnect(QString name)
{
    emit closeConnect(name);
}


void JServer::sendMessage(QString message, QString name)
{
    emit sendMessages(message, name);
}

void JServer::comingConnectionSlot()
{
    qDebug() << "coming connection";
    QTcpSocket *socket = this->nextPendingConnection();//获得新的连接的socket
    QString connectAddr(socket->peerAddress().toString());//获得新的对应用户的地址
    connectAddr.append(":"+QString::number(socket->peerPort()));
    emit newConnect(connectAddr);

    JThread *thread = new JThread(this,this);//创建新的线程
    thread->setSocket(socket);
    thread->setConnectAddr(connectAddr);

    connect(thread, SIGNAL(byteArrived(QString,QString)), this, SIGNAL(byteArrived(QString,QString)));
    connect(thread, SIGNAL(disConnect(QString)), this, SLOT(disConnect(QString)));

    thread->start();
}

void JServer::disConnect(QString name)
{
    emit finishConnect(name);
}
