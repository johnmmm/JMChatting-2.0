/*show window and messages*/

#include "mainwindow.h"
#include <QRegExp>

JWindow::JWindow(QWidget *parent):
    QDialog(parent),
    ui(new Ui::JWindow)
{
    ui->setupUi(this);
    ui->msgTextEdit->setReadOnly(true);
    ui->msgTextEdit->moveCursor(QTextCursor::End);
    ui->msgTextEdit->setTextColor(QColor(255,0,0));
    ui->msgTextEdit->setCurrentFont(QFont("Times", 12));

    m_nameMap = new QMap<QString, QString>;
    m_server = new JServer;
    m_server->listen(QHostAddress::Any, 6666);

    connect(m_server, SIGNAL(byteArrived(QString,QString)), this, SLOT(showMessage(QString,QString)));
    connect(m_server, SIGNAL(newConnect(QString)), this, SLOT(showHello(QString)));
    connect(m_server, SIGNAL(finishConnect(QString)), this, SLOT(finishConnect(QString)));
}

JWindow::~JWindow()
{

}

void JWindow::on_nameList_itemClicked(QListWidgetItem *item)
{
    QString curName(item->text());
    ui->addrLabel->setText(curName);
    ui->nameList->setCurrentItem(item);

    QMap<QString, QString>::Iterator pstr = m_nameMap->find(curName);
    ui->msgTextEdit->setText(*pstr);
    ui->msgTextEdit->moveCursor(QTextCursor::End);
}

void JWindow::on_deleteButton_clicked()
{
    QString nameToDelete(ui->nameList->currentItem()->text());
    m_nameMap->remove(nameToDelete);
    m_server->deleteConnect(nameToDelete);
    ui->msgTextEdit->setText(nameToDelete+"disconnected!");
    ui->addrLabel->clear();

    for(int i = 0; i < ui->nameList->count(); i++)
    {
        if(ui->nameList->item(i)->text() == nameToDelete)
        {
            delete ui->nameList->takeItem(i);
            break;
        }
    }
}

void JWindow::on_clearButton_clicked()
{
    QString curName(ui->nameList->currentItem()->text());
    QMap<QString, QString>::Iterator pstr = m_nameMap->find(curName);
    pstr->clear();

    QDateTime time = QDateTime::currentDateTime();
    QString datetime = time.toString("yyyy-MM-dd hh:mm:ss");
    pstr->append(curName+"clear\n"+datetime+"\n");
    ui->msgTextEdit->setText(*pstr);
}

void JWindow::on_sendButton_clicked()
{
    QString sendName(ui->nameList->currentItem()->text());
    QString message(ui->inputLineEdit->text());
    QString ipAddress(QHostAddress(QHostAddress::LocalHost).toString());

    QMap<QString, QString>::Iterator pstr = m_nameMap->find(sendName);
    pstr->append(ipAddress+":"+QString::number((6000))+"\n");
    pstr->append(message+"\n");

    ui->msgTextEdit->setText(*pstr);
    ui->msgTextEdit->moveCursor(QTextCursor::End);

    ui->inputLineEdit->clear();;
    m_server->sendMessage(message,sendName);
}

void JWindow::showHello(QString name)
{
    QDateTime time = QDateTime::currentDateTime();
    QString curTime = time.toString("yyyy-MM-dd hh:mm:ss");
    QString str(name+" connected! \n ("+curTime+") \n");
    m_nameMap->insert(name,str);
    ui->msgTextEdit->setText(str);

    QListWidgetItem *item = new QListWidgetItem(name);
    ui->nameList->addItem(item);
    ui->nameList->setCurrentItem(item);

    ui->addrLabel->setText(name);

    ui->deleteButton->setEnabled(true);
    ui->clearButton->setEnabled(true);
    ui->sendButton->setEnabled(true);
    ui->sendButton->setDefault(true);
}

void JWindow::showMessage(QString message, QString name)
{
    QMap<QString,QString>::Iterator pstr = m_nameMap->find(name);
    qDebug() << name;
    pstr->append(name + "\n");
    pstr->append(message+"\n");
    //显示信息和显示通信信息
    ui->msgTextEdit->setText(*pstr);
    ui->msgTextEdit->moveCursor(QTextCursor::End);
    ui->addrLabel->setText(name);

    //将当前用户显示为数据来源用户名
    for(int i = 0; i < ui->nameList->count(); i++)
    {
        if(ui->nameList->item(i)->text() == name)
        {
            ui->nameList->setCurrentItem(ui->nameList->item(i));
        }
    }

    //如果这个信息带有一些东西则要进行转发:触发sendMessage
    //先改currentItem,然后解析信息
    auto turnMess = message.split(':');

    //QRegExp rx("^([1]?/d/d?|2[0-1]/d|22[0-3])/.([1]?/d/d?|2[0-4]/d|25[0-5])/.([1]?/d/d?|2[0-4]/d|25[0-5])/.([1]?/d/d?|2[0-4]/d|25[0-5])$");
    //QMap<QString,QString>::Iterator mstr = m_nameMap->find(turnMess[0]+":"+turnMess[1]);
    if(m_nameMap->contains(turnMess[0]+":"+turnMess[1]))
    {
        QString mess;
        for(int i = 0; i < turnMess.length(); i++)
        {
            if(i < turnMess.length()-1)
            {
                mess.append(turnMess[i]+":");
                continue;
            }
            if(i == turnMess.length()-1)
                mess.append(turnMess[i]);
        }
        qDebug() << "turn";
        m_server->sendMessage(mess,turnMess[0]+":"+turnMess[1]);
    }

}

void JWindow::finishConnect(QString nameAddr)
{
    m_nameMap->remove(nameAddr);
    ui->msgTextEdit->setText(nameAddr+"disconnected!");
    ui->addrLabel->clear();

    for(int i = 0; i < ui->nameList->count(); i++)
    {
        if(ui->nameList->item(i)->text() == nameAddr)
        {
            delete ui->nameList->takeItem(i);
            break;
        }
    }

    if(ui->nameList->count() == 0)
    {
        ui->deleteButton->setEnabled(false);
        ui->clearButton->setEnabled(false);
        ui->sendButton->setEnabled(false);
    }
}
