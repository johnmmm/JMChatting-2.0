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
#include "client.h"
#include "ui_mainwindow.h"

class JClient;

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

    void showHello(QString name);
    void showMessage(QString message, QString connectAddr, QString localAddr);
    void finishConnect(QString nameAddr);
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_clearButton_clicked();
    void on_nameList_itemChanged(QListWidgetItem *item);
    void on_sendButton_clicked();

private:
    Ui::JWindow *ui;

    JClient *m_client;               //主窗口的用户服务器
    QMap<QString, QString> *m_nameMap;//用户通信信息
};

#endif // MAINWINDOW_H
