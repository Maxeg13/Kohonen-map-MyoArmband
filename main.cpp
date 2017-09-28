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

    MainWindow SignalMW;
    SignalMW.resize(QSize(600,300));

    SignalMW.setWindowTitle("Myographic signals");

    //    KohonenMW.show();
    SignalMW.show();
    //    w.kohonenWidget->show();
    return a.exec();
}
