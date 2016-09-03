#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTcpSocket>
#include "server.h"

class JServer;

class JThread : public QThread
{
    Q_OBJECT

public:
    JServer *ser;

    JThread(QObject *parent, JServer *ser);
    ~JThread();
    void run();
    void setSocket(QTcpSocket *socket);
    void setConnectAddr(QString connectAddr);

signals:
    void error(QTcpSocket::SocketError socketError);
    void byteArrived(QString message, QString m_connectAddr);
    void disConnect(QString m_connectAddr);

private slots:
    void closeConnect(QString name);
    void disConnect();
    void receiveData();
    void sendMessage(QString message, QString name);

private:
    QTcpSocket *m_tcpsocket;
    QString m_connectAddr;
    quint16 m_blockSize;

};

#endif // THREAD_H
