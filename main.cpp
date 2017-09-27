#include "mainwindow.h"
#include "kohonenwidget.h"
#include <QApplication>
#include <QDebug>
#include <QtGui>
#include <QPushButton>
#include <QCoreApplication>
#include <vector>
#include "drawing_perc.h"
#include "perceptron.h"
perceptron* perc;
//myCurve* percCurve;

using namespace std;
vector<vector<float>> dataTest;
vector<vector<float>> dataL;

int main(int argc, char *argv[])
{



    dataL.resize(2);
    int learnSize=15;
    dataL[0].resize(learnSize);
    dataL[1].resize(learnSize);
    for(int i=0;i<dataL[0].size();i++)
    {
        dataL[0][i]=((i-learnSize/2)/70.*3);
        dataL[1][i]= (dataL[0][i]-0.5)*(( dataL[0][i]>0)?1:(-1));
    }
//    curveLearn=new myCurve(dataL,d_plot,"Target",QColor(0,0,0,0),Qt::red);
//    curveLearn->drawing();

    vector<int> constr;
    constr.push_back(1);
    constr.push_back(5);
    constr.push_back(5);
    constr.push_back(1);//output
    //    perc=new perceptron(constr);

//    myCurve *curveLearn, *curveTest;


    QApplication a(argc, argv);

    MainWindow SignalMW;
    SignalMW.resize(QSize(600,300));
    //    QMainWindow KohonenMW;
    //    KohonenMW.resize(QSize(590,550));
    //    KohonenWidget KW(SignalMW.featureOut);

    //    QGridLayout* GL=new QGridLayout();
    //    QWidget *centralWidget1=new QWidget();
    //    centralWidget1->setLayout(GL);

    //    int buttons_n=9;
    //    GL->addWidget(&KW,1,1,1,buttons_n);
    //    GL->addWidget(KW.learnB1,2,1);
    //    GL->addWidget(KW.learnB2,2,2);
    //    GL->addWidget(KW.learnB3,2,3);
    //    GL->addWidget(KW.learnB4,2,4);
    //    GL->addWidget(KW.learnB5,2,5);
    //    GL->addWidget(KW.learnB6,2,6);
    //    GL->addWidget(KW.learnB7,2,7);
    //    GL->addWidget(KW.learnB8,3,1);
    //    GL->addWidget(KW.rstLearningB,3,2);
    //    GL->addWidget(KW.corB,3,3);
    //    GL->addWidget(KW.L_E,3,4);
    //    GL->addWidget(KW.L_E_F,3,5);

    //    KohonenMW.setCentralWidget(centralWidget1);
    //    QObject::connect(&SignalMW,SIGNAL(featureOutSignal(std::vector<float>)),&KW,SLOT(refresh(std::vector<float>)));
    //    //QObject::connect(&KW.learnB1,SIGNAL(released()),&KW,SLOT(learning_1()));
    //    QObject::connect((KW.L_E),SIGNAL(editingFinished()),&KW,SLOT(pushString()));
    //    QObject::connect(&KW,SIGNAL(pushStringS(QString)),&SignalMW,SLOT(reconnect(QString)));
    //    QObject::connect(&KW,SIGNAL(corSignal()),&SignalMW,SLOT(getCor()));

    //    KohonenMW.setWindowTitle("Kohonen hex-top Map");
    SignalMW.setWindowTitle("Myographic signals");

    //    KohonenMW.show();
    SignalMW.show();
    //    w.kohonenWidget->show();
    return a.exec();
}
