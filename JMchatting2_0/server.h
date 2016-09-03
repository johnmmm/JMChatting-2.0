#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QString>
#include "mainwindow.h"
#include "thread.h"

class JThread;

class JServer : public QTcpServer
{
    Q_OBJECT

public:
    JServer(QObject *parent = 0);
    ~JServer();
    void deleteConnect(QString name);
    void sendMessage(QString message, QString name);

signals:
    void newConnect(QString connectAddr);
    void closeConnect(QString name);//主动关闭连接信号
    void finishConnect(QString name);//被动关闭连接信号
    void byteArrived(QString message, QString name);
    void sendMessages(QString message, QString name);

private slots:
    void comingConnectionSlot();
    void disConnect(QString name);

};

#endif // SERVER_H
