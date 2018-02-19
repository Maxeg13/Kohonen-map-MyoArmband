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
    h.resize(3);
    KohonenWidget KW(h);
    KW.show();


    for(int i=0;i<1000;i++)
    {
        h.resize(0);
        for(int j=0;j<3;j++)
            h.push_back((rand()%100)*0.001*(HW.hand.max[j]-HW.hand.min[j])+HW.hand.min[j]);
        KW.data_learn.push_back(h);
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
    GL->addWidget(KW.L_E_F,2,4);
    //        L_E.setText("COM6");


    KohonenMW.setCentralWidget(centralWidget1);
    QObject::connect(&HW,SIGNAL(featureOutSignal(std::vector<float>)),&KW,SLOT(refresh(std::vector<float>)));
    QObject::connect((KW.L_E),SIGNAL(editingFinished()),&KW,SLOT(pushString()));


    KohonenMW.setWindowTitle("Kohonen hex-top Map");
    //    SignalMW.setWindowTitle("Myographic signals");

    emit HW.featureOutSignal(HW.handData);
    KohonenMW.show();
    //    SignalMW.show();
    //    w.kohonenWidget->show();
    return a.exec();
}
