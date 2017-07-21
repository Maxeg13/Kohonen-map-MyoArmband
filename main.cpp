#include "mainwindow.h"
#include "kohonenwidget.h"
#include <QApplication>
#include <QDebug>
#include <QtGui>
#include <QPushButton>
#include <QCoreApplication>
#include <windows.h>



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    MainWindow SignalMW;
    SignalMW.resize(QSize(600,300));
    QMainWindow KohonenMW;
    KohonenMW.resize(QSize(590,550));
    KohonenWidget KW;



    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);

    int buttons_n=8;
    GL->addWidget(&KW,1,1,1,buttons_n);
    GL->addWidget(KW.learnB1,2,1);
    GL->addWidget(KW.learnB2,2,2);
    GL->addWidget(KW.learnB3,2,3);
    GL->addWidget(KW.learnB4,2,4);
    GL->addWidget(KW.learnB5,2,5);
    GL->addWidget(KW.learnB6,2,6);
    GL->addWidget(KW.rstLearningB,2,7);
    GL->addWidget(KW.L_E,2,buttons_n);
    //    L_E.setText("COM6");


    KohonenMW.setCentralWidget(centralWidget1);
    QObject::connect(&SignalMW,SIGNAL(featureOutSignal(QVector<float>)),&KW,SLOT(refresh(QVector<float>)));
    //QObject::connect(&KW.learnB1,SIGNAL(released()),&KW,SLOT(learning_1()));
    QObject::connect((KW.L_E),SIGNAL(editingFinished()),&KW,SLOT(pushString()));
    QObject::connect(&KW,SIGNAL(pushStringS(QString)),&SignalMW,SLOT(reconnect(QString)));

    KohonenMW.setWindowTitle("Kohonen hex-top Map");
    SignalMW.setWindowTitle("Myographic signals");

    KohonenMW.show();
    SignalMW.show();
    //    w.kohonenWidget->show();
    return a.exec();
}
