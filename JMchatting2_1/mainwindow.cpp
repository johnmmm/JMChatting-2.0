#include "mainwindow.h"

JWindow::JWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JWindow)
{
    ui->setupUi(this);
    ui->msgTextEdit->setReadOnly(true);
    ui->msgTextEdit->moveCursor(QTextCursor::End);
    ui->msgTextEdit->setTextColor(QColor(255,0,0));
    ui->msgTextEdit->setCurrentFont(QFont("Times", 12));

    m_nameMap = new QMap<QString, QString>;
    m_client = new JClient;

    connect(m_client, SIGNAL(newConnect(QString)), this, SLOT(showHello(QString)));
    connect(m_client, SIGNAL(finishConnect(QString)), this, SLOT(finishConnect(QString)));
    connect(m_client, SIGNAL(byteArrived(QString,QString,QString)), this, SLOT(showMessage(QString,QString,QString)));
}

JWindow::~JWindow()
{

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

void JWindow::showMessage(QString message, QString connectAddr, QString localAddr)
{
    QMap<QString,QString>::Iterator pstr = m_nameMap->find(localAddr);
    pstr->append(connectAddr+":\n");
    pstr->append(message+"\n");
    //显示信息和显示通信信息
    ui->msgTextEdit->setText(*pstr);
    ui->msgTextEdit->moveCursor(QTextCursor::End);
    ui->addrLabel->setText(localAddr);

    //将当前用户显示为数据来源用户名
    for(int i = 0; i < ui->nameList->count(); i++)
    {
        if(ui->nameList->item(i)->text() == localAddr)
        {
            ui->nameList->setCurrentItem(ui->nameList->item(i));
        }
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


void JWindow::on_addButton_clicked()
{
    bool ok;
    QString nameAddr = QInputDialog::getText(this, tr("Connect the server"),
                                             tr("Please input your IP. For example: 127.0.0.1"), QLineEdit::Normal, tr("127.0.0.1"), &ok);

    if(ok && !nameAddr.isEmpty())
        m_client->connectToHost(nameAddr);
}

void JWindow::on_deleteButton_clicked()
{
    QString nameToDelete(ui->nameList->currentItem()->text());
    m_nameMap->remove(nameToDelete);
    m_client->deleteConnect(nameToDelete);
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

void JWindow::on_nameList_itemChanged(QListWidgetItem *item)
{
    QString curName(item->text());
    ui->addrLabel->setText(curName);
    ui->nameList->setCurrentItem(item);

    QMap<QString, QString>::Iterator pstr = m_nameMap->find(curName);
    ui->msgTextEdit->setText(*pstr);
    ui->msgTextEdit->moveCursor(QTextCursor::End);
}

void JWindow::on_sendButton_clicked()
{
    if(!ui->inputLineEdit->text().isEmpty())
    {
        QString sendName(ui->nameList->currentItem()->text());
        QString sendstr(ui->inputLineEdit->text());

        QMap<QString, QString>::Iterator pstr = m_nameMap->find(sendName);
        pstr->append(sendName+"\n");
        pstr->append(sendstr+"\n");
        ui->msgTextEdit->setText(*pstr);
        ui->msgTextEdit->moveCursor(QTextCursor::End);

        ui->inputLineEdit->clear();
        m_client->sendMessage(sendstr, sendName);
    }
}
