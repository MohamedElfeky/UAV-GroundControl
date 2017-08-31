#include <QtGui/QApplication>
 #include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK")); //·����֧������
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK")); //QString֧������
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK")); //string֧������
    MainWindow w;
    w.show();
    
    return a.exec();
}
