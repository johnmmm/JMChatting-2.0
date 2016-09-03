#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QDebug>
#include <QTcpSocket>
#include <QThread>

#include "thread.h"

class JClient : public QTcpSocket
{
    Q_OBJECT

public:
    JClient();
    ~JClient();
    void connectToHost(QString hostAddr);
    void deleteConnect(QString name);
    void sendMessage(QString message, QString name);

signals:
    void newConnect(QString connectAddr);
    void closeConnect(QString name);//主动关闭连接信号
    void finishConnect(QString name);//被动关闭连接信号
    void byteArrived(QString message, QString connectAddr, QString localAddr);
    void sendMessages(QString message, QString name);

private slots:
    void disconnect(QString name);

private:
    int m_port;
    QTcpSocket *m_tcpSocket;
};

#endif // CLIENT_H
