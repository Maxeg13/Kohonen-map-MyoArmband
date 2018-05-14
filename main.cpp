#include "mainwindow.h"
#include "kohonenwidget.h"
#include <QApplication>
#include <QDebug>
#include <QtGui>
#include <QPushButton>
#include <QCoreApplication>
//#include <windows.h>



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    MainWindow SignalMW;
    SignalMW.resize(QSize(600,300));
    QMainWindow KohonenMW;
    KohonenMW.resize(QSize(590,600));
    qDebug()<< SignalMW.featureOut;
    KohonenWidget KW(SignalMW.featureOut);



    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);

    int buttons_n=9;
    GL->addWidget(&KW,1,1,1,buttons_n);
    GL->addWidget(KW.saveB,2,1);
    GL->addWidget(KW.rstB,2,2);
    GL->addWidget(KW.learningB,2,3);
    GL->addWidget(KW.corB,2,4);
    GL->addWidget(KW.L_E,2,5);
    GL->addWidget(KW.L_E_rad,2,6);
    GL->addWidget(KW.L_E_ind1,3,1);
    GL->addWidget(KW.L_E_ind2,3,2);
    GL->addWidget(KW.connectB,3,3);
    GL->addWidget(KW.L_E_shift1,4,1);
    GL->addWidget(KW.L_E_shift2,4,2);
    //    L_E.setText("COM6");

    QObject::connect(&SignalMW,SIGNAL(sign_close()),&a,SLOT(closeAllWindows()));
    KohonenMW.setCentralWidget(centralWidget1);
    QObject::connect(&SignalMW,SIGNAL(featureOutSignal( vector<float>)),&KW,SLOT(refresh( vector<float>)));
    //QObject::connect(&KW.learnB1,SIGNAL(released()),&KW,SLOT(learning_1()));
    QObject::connect((KW.L_E),SIGNAL(editingFinished()),&KW,SLOT(pushString()));
    QObject::connect(&KW,SIGNAL(pushStringS(QString)),&SignalMW,SLOT(reconnect(QString)));
    QObject::connect(&KW,SIGNAL(corSignal()),&SignalMW,SLOT(getCor()));
    QObject::connect(KW.connectB,SIGNAL(released()),&SignalMW,SLOT(connectMyo()));
    QObject::connect(KW.L_E_shift1,SIGNAL(editingFinished()),&KW,SLOT(SHIFT()));
    QObject::connect(KW.L_E_shift2,SIGNAL(editingFinished()),&KW,SLOT(SHIFT()));
//    QObject::connect();

    KohonenMW.setWindowTitle("Kohonen hex-top Map");
    SignalMW.setWindowTitle("Myographic signals");

    KohonenMW.show();
    SignalMW.show();
    //    w.kohonenWidget->show();
     a.exec();
//     a.closeAllWindows();
}
