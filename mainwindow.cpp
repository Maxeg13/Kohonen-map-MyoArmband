//works with KULER_myolib
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawing.h"
#include "headers.h"
//#include ""
#include "stdafx.h"
#include "targetver.h"
#include <Windows.h>


#include "layer_koh.h"
#include "serial.h"
#include <QThread>
//#include "MYO_lib.h"
#include "stand_dev.h"

using namespace std;


QThread* thread;
Serial hSerial;

int axisScale=10000;

int8_t EMG1;
standartDev STD[2];
frBuHp2 FBH[2];
//bandPassFr BPF[2];
matchedFr MF[2];
lowPassFr LPF[2];
lowPassFr2 LPF2[2];
matchedFrV MFV[2];
integrator INTEGR[2];
featureExtr1 FE1[2];
WillisonAmp WA[2];
int bufShowSize=1500;
QTimer *timer;
QPainter *painter;
myCurve *curveTest[2], *curveFeature1[2], *curveFeature2[2], *curveFeature3[2], *curveFeature4[2];
QVector<QVector <QVector<float>>> dataEMG, featureEMG1, featureEMG2, featureEMG3, featureEMG4;
int ind_c[2];



serial_obj::serial_obj(QString qstr)
{
    std::string str1=qstr.toUtf8().constData();;
    std::wstring str(str1.begin(),str1.end());

    hSerial.InitCOM(str.c_str());//was L"COM5"
    //    featureOut.resize(3);
    featureOut.resize(8);
    for(int i=0;i<featureOut.size();i++)
        featureOut[i]=1;

}

void serial_obj::init(QString qstr)
{
    std::string str1=qstr.toUtf8().constData();;
    std::wstring str(str1.begin(),str1.end());

    hSerial.InitCOM(str.c_str());//was L"COM5"
    //    featureOut.resize(3);
}

void serial_obj::close()
{
    hSerial.close();
}
serial_obj::~serial_obj()
{};
void serial_obj::doWork()
{

    while(1)
    {

        bool readVarON;
        readVar=(int8_t)hSerial.ReadCOM(readVarON);

        if(readVarON)
        {
            int presc=3;
            gottenVar[1]=gottenVar[0];
            gottenVar[0]=readVar;
            ptr++;
            ptr%=presc;
            if(((gottenVar[0]!=2)&&(gottenVar[0]!=1))&&(ptr==0))
            {
                //alert!!!
                ptr++;
                ptr%=presc;
            }
            else
            {
                if((ptr==1)&&(gottenVar[1]==1))
                {
                    ind_c[0]=(ind_c[0]+1)%dataEMG[0][1].size();

                    EMG1=readVar;

                    dataEMG[0][1][ind_c[0]]=
                            // FBH[0](
                            8*readVar;//);

                    featureOut[0]=featureEMG1[0][1][ind_c[0]]=FE1[0](EMG1)/20;
                    featureOut[1]=featureEMG2[0][1][ind_c[0]]=LPF[0](STD[0](EMG1));
                    featureOut[2]=featureEMG3[0][1][ind_c[0]]=LPF[1](WA[0](EMG1));
                    featureOut[3]=featureEMG4[0][1][ind_c[0]]=(400*LPF2[0]((killRange(MFV[0](EMG1),30))));;
                    //emit(learnSig())
                }
                if((ptr==1)&&(gottenVar[1]==2))
                {
                    ind_c[1]=(ind_c[1]+1)%dataEMG[1][1].size();

                    EMG1=readVar;

                    dataEMG[1][1][ind_c[1]]=
                            // FBH[0](
                            8*readVar;//);

                    featureOut[4]=featureEMG1[1][1][ind_c[1]]=FE1[1](EMG1)/20;
                    featureOut[5]=featureEMG2[1][1][ind_c[1]]=LPF[2](STD[1](EMG1));
                    featureOut[6]=featureEMG3[1][1][ind_c[1]]=LPF[3](WA[1](EMG1));
                    featureOut[7]=featureEMG4[1][1][ind_c[1]]=(400*LPF2[1]((killRange(MFV[1](EMG1),30))));;
                    //emit(learnSig())
                }

            }
        }
    }
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    dataEMG.resize(2);
    featureEMG1.resize(2);
    featureEMG2.resize(2);
    featureEMG3.resize(2);
    featureEMG4.resize(2);
    for(int i_pl=0;i_pl<2;i_pl++)
    {
        d_plot[i_pl] = new QwtPlot(this);
        //________________________
        //    setCentralWidget(d_plot);
        drawingInit(d_plot[i_pl]);
            d_plot[i_pl]->setAxisScale(QwtPlot::yLeft,-axisScale,axisScale);

        curveTest[i_pl]=new myCurve(bufShowSize, dataEMG[i_pl],d_plot[i_pl],"EMG_1",Qt::black,Qt::black,ind_c[i_pl]);

        curveFeature1[i_pl]=new myCurve(bufShowSize,featureEMG1[i_pl],d_plot[i_pl],"bipolar feature1",Qt::red,Qt::black,ind_c[i_pl]);

        curveFeature2[i_pl]=new myCurve(bufShowSize, featureEMG2[i_pl],d_plot[i_pl],"force feature",Qt::green,Qt::black,ind_c[i_pl]);

        curveFeature3[i_pl]=new myCurve(bufShowSize, featureEMG3[i_pl],d_plot[i_pl],"Willison's feature2",Qt::blue,Qt::black,ind_c[i_pl]);

        curveFeature4[i_pl]=new myCurve(bufShowSize, featureEMG4[i_pl],d_plot[i_pl],"bipolar feature2",Qt::red,Qt::black,ind_c[i_pl]);
    }


    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);

    GL->addWidget(d_plot[0],1,1);
    GL->addWidget(d_plot[1],2,1);

    setCentralWidget(centralWidget1);



    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    timer->start(40);

    QThread* thread = new QThread( );
    SO=new serial_obj("COM5");
    SO->moveToThread(thread);

    //    connect(this,SIGNAL(featureOutSignal(QVector<float>)),this,SLOT(getFeature(QVector<float>)));
    connect(thread,SIGNAL(started()),SO,SLOT(doWork()));
    thread->start();

}





void MainWindow::drawing()
{
    for(int p_ind=0;p_ind<2;p_ind++)
    {
        curveTest[p_ind]->signalDrawing();
        curveFeature1[p_ind]->signalDrawing();
        curveFeature2[p_ind]->signalDrawing();
        curveFeature3[p_ind]->signalDrawing();
        curveFeature4[p_ind]->signalDrawing();
    }
    featureOut=SO->featureOut;
    //    qDebug()<<SO->featureOut[0];
    emit featureOutSignal(featureOut);
}

void MainWindow::getFeature(QVector<float> x)
{
    qDebug()<<x[0];
}

void MainWindow::reconnect(QString s)
{
qDebug()<<s;
SO->close();
//delete SO;
SO->init(s);
}


void MainWindow::drawingInit(QwtPlot* d_plot)
{
    //        setCentralWidget(MW);


    //d_plot->autoRefresh();
    d_plot->setAutoReplot();
    //_______232

    // настройка функций
    QwtPlotPicker *d_picker =
            new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft, // ассоциация с осями
                QwtPlotPicker::CrossRubberBand, // стиль перпендикулярных линий
                QwtPicker::ActiveOnly, // включение/выключение
                d_plot->canvas() ); // ассоциация с полем
    // Цвет перпендикулярных линий
    d_picker->setRubberBandPen( QColor( Qt::red ) );

    // цвет координат положения указателя
    d_picker->setTrackerPen( QColor( Qt::black ) );

    // непосредственное включение вышеописанных функций
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );

    // Включить возможность приближения/удаления графика
    // #include <qwt_plot_magnifier.h>
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(d_plot->canvas());
    // клавиша, активирующая приближение/удаление
    magnifier->setMouseButton(Qt::MidButton);
    // Включить возможность перемещения по графику
    // #include <qwt_plot_panner.h>
    QwtPlotPanner *d_panner = new QwtPlotPanner( d_plot->canvas() );
    // клавиша, активирующая перемещение
    d_panner->setMouseButton( Qt::RightButton );
    // Включить отображение координат курсора и двух перпендикулярных
    // линий в месте его отображения
    // #include <qwt_plot_picker.h>
    //    d_plot->setTitle( "My perceptron demonstration" ); // заголовок
    d_plot->setCanvasBackground( Qt::white ); // цвет фона

    // Параметры осей координат
    d_plot->setAxisTitle(QwtPlot::yLeft, "EMG, mkV");
    d_plot->setAxisTitle(QwtPlot::xBottom, "time");
    d_plot->insertLegend( new QwtLegend() );


    // Включить сетку
    // #include <qwt_plot_grid.h>
    QwtPlotGrid *grid = new QwtPlotGrid(); //

    grid->setMajorPen(QPen( Qt::gray, 2 )); // цвет линий и толщина
    grid->attach( d_plot ); // добавить сетку к полю графика
}

MainWindow::~MainWindow()
{
    //    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *e)
{

}

