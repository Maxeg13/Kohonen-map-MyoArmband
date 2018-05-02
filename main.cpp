#include "mainwindow.h"
#include "handwindow.h"
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

    HandWindow HW;
    HW.show();
    //    MainWindow SignalMW;
    //    SignalMW.resize(QSize(600,300));
    QMainWindow KohonenMW;
    KohonenMW.resize(QSize(590,550));
    //    qDebug()<< SignalMW.featureOut;
    vector<float> h;
    ////
    h.resize(2);
    KohonenWidget KW(h);
//    KW.RH=HW.hand;
    KW.show();


    for(int i=0;i<1000;i++)
    {
        h.resize(0);
        for(int j=0;j<2;j++)
            ////
            h.push_back((rand()%100)*0.01*(HW.hand.max[j]-HW.hand.min[j])+HW.hand.min[j]);
        KW.data_learn.push_back(h);
    }

    for(int i=0;i<1000;i++)
    {
        HW.setAngles(KW.data_learn[i]);
        HW.hand.get_bones();
        KW.LK->set_on(((HW.hand.origin[2]-HW.hand.origin[0])*5+QPoint(0,800)));
    }

    KW.rst();

    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);

    int buttons_n=9;
    GL->addWidget(&KW,1,1,1,buttons_n);
    GL->addWidget(KW.saveB,2,1);
    GL->addWidget(KW.rstLearningB,2,2);
    GL->addWidget(KW.corB,2,3);
    //        GL->addWidget(KW.L_E,2,4);
    GL->addWidget(KW.L_E_1,3,1);
    GL->addWidget(KW.L_E_2,3,2);
    //        L_E.setText("COM6");


    KohonenMW.setCentralWidget(centralWidget1);
    QObject::connect(&HW,SIGNAL(featureOutSignal(vector<float>)),&KW,SLOT(refresh(vector<float>)));
    QObject::connect((KW.L_E),SIGNAL(editingFinished()),&KW,SLOT(pushString()));
    QObject::connect(&KW,SIGNAL(pushWeight(vector<float>)),&HW,SLOT(setConf(vector<float>)));

    KohonenMW.setWindowTitle("Kohonen hex-top Map");
    //    SignalMW.setWindowTitle("Myographic signals");

    emit HW.featureOutSignal(HW.handData);
    KohonenMW.show();
    //    SignalMW.show();
    //    w.kohonenWidget->show();
    return a.exec();
}
