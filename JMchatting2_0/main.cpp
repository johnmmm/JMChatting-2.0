/*This one is a server*/

#include <QApplication>
#include <QtCore>
#include <QTextCodec>
#include "mainwindow.h"

int main (int argc, char* argv[])
{
    QApplication a(argc,argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    JWindow win;
    win.show();

    return a.exec();
}
