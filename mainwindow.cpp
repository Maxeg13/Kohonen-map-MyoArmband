//works with KULER_myolib
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawing.h"
#include "headers.h"

#include "stdafx.h"
#include "targetver.h"
#include <Windows.h>

#include "layer_koh.h"
#include "serial.h"
#include <QThread>
#include "ab_practice/include/MYO_lib.h"
#include "ab_practice/include/stand_dev.h"

using namespace std;


QThread* thread;
HANDLE hSerial;
int readVar;
int axisScale=10000;

int8_t EMG1;
standartDev STD[2];
frBuHp2 FBH[2];
bandPassFr BPF[2];
matchedFr MF[2];
lowPassFr LPF[2];
integrator INTEGR[2];
featureExtr1 FE1[2];
WillisonAmp WA[2];
int bufShowSize=1500;
QTimer *timer;
QPainter *painter;
myCurve *curveTest, *curveFeature1, *curveFeature2, *curveFeature3;
QVector <QVector<float>> dataEMG1, dataEMG2,featureEMG11, featureEMG12, featureEMG13;
int ind_c[2];


serial_obj::serial_obj()
{
    InitCOM(hSerial,L"COM5");
//    featureOut.resize(3);
    featureOut.resize(3);
    for(int i=0;i<featureOut.size();i++)
        featureOut[i]=1;

}
serial_obj::~serial_obj()
{};
void serial_obj::doWork()
{

    while(1)
    {

        bool readVarON;
        readVar=(int8_t)ReadCOM(hSerial,readVarON);

        if(readVarON)
        {
            int presc=3;
            ptr++;
            ptr%=presc;
            if((readVar!=2)&&(ptr==0))
            {
                //alert!!!
                ptr++;
                ptr%=presc;
            }
            else
            {
                if(ptr==1)
                {
                    ind_c[0]=(ind_c[0]+1)%dataEMG1[1].size();

                    EMG1=readVar;

                    dataEMG1[1][ind_c[0]]=
                            // FBH[0](
                            8*readVar;//);

                    featureOut[0]=featureEMG11[1][ind_c[0]]=FE1[0](EMG1);
                    featureOut[1]=featureEMG12[1][ind_c[0]]=14*LPF[0](STD[0](EMG1));
//                    featureOut[2]=featureEMG13[1][ind_c[0]]=LPF[1](WA[0](EMG1));
                    featureOut[2]=featureEMG13[1][ind_c[0]]=FE1[1](-EMG1);
//emit(learnSig())
                }
            }
        }
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    d_plot = new QwtPlot(this);

    //________________________
    setCentralWidget(d_plot);
    drawingInit(d_plot);

    d_plot->setAxisScale(QwtPlot::yLeft,-axisScale,axisScale);


    curveTest=new myCurve(bufShowSize, dataEMG1,d_plot,"EMG_1",Qt::black,Qt::black,ind_c[0]);

    curveFeature1=new myCurve(bufShowSize,featureEMG11,d_plot,"bipolar feature",Qt::red,Qt::black,ind_c[0]);

    curveFeature2=new myCurve(bufShowSize, featureEMG12,d_plot,"force feature",Qt::green,Qt::black,ind_c[0]);

    curveFeature3=new myCurve(bufShowSize, featureEMG13,d_plot,"Willison's feature",Qt::blue,Qt::black,ind_c[0]);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    timer->start(40);

    QThread* thread = new QThread( );
    SO=new serial_obj();
    SO->moveToThread(thread);

//    connect(this,SIGNAL(featureOutSignal(QVector<float>)),this,SLOT(getFeature(QVector<float>)));
    connect(thread,SIGNAL(started()),SO,SLOT(doWork()));
    thread->start();

}





void MainWindow::drawing()
{
    curveTest->signalDrawing();
    curveFeature1->signalDrawing();
    curveFeature2->signalDrawing();
    curveFeature3->signalDrawing();

    featureOut=SO->featureOut;
//    qDebug()<<SO->featureOut[0];
    emit featureOutSignal(featureOut);
}

void MainWindow::getFeature(QVector<float> x)
{
    qDebug()<<x[0];
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

