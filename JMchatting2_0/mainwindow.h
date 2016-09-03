#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMap>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>
#include <QInputDialog>
#include <QDateTime>
#include "server.h"
#include "ui_mainwindow.h"

class JServer;

namespace Ui
{
class JWindow;
}

class JWindow : public QDialog
{
    Q_OBJECT

public:
    JWindow(QWidget *parent = 0);
    ~JWindow();

private slots:
    void on_nameList_itemClicked(QListWidgetItem *item);
    void on_deleteButton_clicked();
    void on_clearButton_clicked();
    void on_sendButton_clicked();
    void showHello(QString name);
    void showMessage(QString message, QString name);
    void finishConnect(QString nameAddr);

private:
    Ui::JWindow *ui;

    JServer  *m_server;               //主窗口的用户服务器
    QMap<QString, QString> *m_nameMap;//用户通信信息
};


#endif // MAINWINDOW_H
