#include "mainwindow.h"
#include "kohonenwidget.h"
#include <QApplication>
#include <QDebug>
#include <QtGui>
#include <QPushButton>
#include <QCoreApplication>
#include <vector>

using namespace std;
int main(int argc, char *argv[])
{



    QApplication a(argc, argv);

//    QThread* thread;
//    QString qstr("COM5");
//    SO=new serial_obj(qstr);
//    thread = new QThread( );
//    SO->moveToThread(thread);
////    connect(thread,SIGNAL())
//    QObject::connect(thread,SIGNAL(started()),SO,SLOT(doWork()));
//    thread->start();

    MainWindow SignalMW;
    SignalMW.resize(QSize(600,300));
    SignalMW.setWindowTitle("Myographic signals");
    SignalMW.show();
    return a.exec();
}
