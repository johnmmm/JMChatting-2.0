#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QMessageBox>
#include "client.h"

class JClient;

class JThread : public QThread
{
    Q_OBJECT

public:
    JClient *jh;
    JThread(QObject *parent, JClient *jh);
    ~JThread();
    void run();
    void setSocket(QTcpSocket *socket);

signals:
    void disConnect(QString name);
    void newConnect(QString name);
    void byteArrived(QString message, QString connectAddr, QString localAddr);

private slots:
    void socketConnect();
    void disconnect();
    void closeConnect(QString name);
    void receiveData();
    void sendMessage(QString message, QString name);
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *m_tcpSocket;
    QString m_localAddr;
    quint16 m_blockSize;

};





#endif // THREAD_H
