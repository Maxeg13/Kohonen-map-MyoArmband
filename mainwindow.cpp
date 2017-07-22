#include "mainwindow.h"

//SERIAL may be defined in mainwindow.h
#ifdef SERIAL
#include "serialqobj.h"
#else
#include "datacollector.h"
#endif


#include "drawing.h"
#include "headers.h"
#include "stdafx.h"
#include "targetver.h"
#include <Windows.h>
#include "layer_koh.h"

#include <QThread>


using namespace std;


QThread* thread;

#ifdef SERIAL

int axisScale=10000;
#else
int axisScale=1000;
DataCollector collector;
#endif

int bufShowSize=1500;
QTimer *timer;
QTimer *timerMyo;
QPainter *painter;

#ifdef SERIAL
myCurve *curveTest[2], *curveFeature1[2], *curveFeature2[2], *curveFeature3[2], *curveFeature4[2];
#else
myCurve *curveTest[8], *curveFeature1[8];
#endif

QVector <QVector<float>> dataEMG;
QVector <QVector <QVector<float>>> featureEMG;
int ind_c[8];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{



    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);

#ifdef SERIAL
    dataEMG.resize(2);
    featureEMG.resize(2);
    featureEMG.resize(2);
    featureEMG.resize(2);
    featureEMG.resize(2);
    featureEMG[0].resize(4);
    featureEMG[1].resize(4);

    for(int i_pl=0;i_pl<2;i_pl++)
    {
        d_plot[i_pl] = new QwtPlot(this);
        //________________________
        //    setCentralWidget(d_plot);
        drawingInit(d_plot[i_pl]);
        d_plot[i_pl]->setAxisScale(QwtPlot::yLeft,-axisScale,axisScale);

        curveTest[i_pl]=new myCurve(bufShowSize, dataEMG[i_pl],d_plot[i_pl],"EMG_1",Qt::black,Qt::black,ind_c[i_pl]);

        curveFeature1[i_pl]=new myCurve(bufShowSize,featureEMG[i_pl][0],d_plot[i_pl],"bipolar feature1",Qt::red,Qt::black,ind_c[i_pl]);

        curveFeature2[i_pl]=new myCurve(bufShowSize, featureEMG[i_pl][1],d_plot[i_pl],"force feature",Qt::green,Qt::black,ind_c[i_pl]);

        curveFeature3[i_pl]=new myCurve(bufShowSize, featureEMG[i_pl][2],d_plot[i_pl],"Willison's feature2",Qt::blue,Qt::black,ind_c[i_pl]);

        curveFeature4[i_pl]=new myCurve(bufShowSize, featureEMG[i_pl][3],d_plot[i_pl],"bipolar feature2",Qt::red,Qt::black,ind_c[i_pl]);
    }

    GL->addWidget(d_plot[0],1,1);
    GL->addWidget(d_plot[1],2,1);
#else
    dataEMG.resize(8);
    featureEMG.resize(8);

    for( int i_pl=0;i_pl<8;i_pl++)
    {
        featureEMG[i_pl].resize(1);

        d_plot[i_pl] = new QwtPlot(this);
        drawingInit(d_plot[i_pl]);
        d_plot[i_pl]->setAxisScale(QwtPlot::yLeft,-axisScale,axisScale);
        GL->addWidget(d_plot[i_pl],1+i_pl,1);
    }
#endif






    setCentralWidget(centralWidget1);



    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    timer->start(40);



#ifdef SERIAL
    QThread* thread = new QThread( );
    SO=new serial_obj("COM5",dataEMG,featureEMG,ind_c);
    SO->moveToThread(thread);
    connect(thread,SIGNAL(started()),SO,SLOT(doWork()));
    thread->start();
#else
    timerMyo = new QTimer(this);
    connect(timerMyo, SIGNAL(timeout()), this, SLOT(kickMyo()));
    timerMyo->start(4);
    connect(&collector.qdc,SIGNAL(EMG(QVector<float>)),this,SLOT(getEMG(QVector<float>)));
#endif
}


void MainWindow::kickMyo()
{
#ifndef SERIAL
    collector.kick(10);
#endif
}

void MainWindow::getEMG(QVector<float> x)
{
    for (int i=0;i<8;i++)
    {
    dataEMG[i][ind_c[i]]=x[i];

    ind_c[i]=(ind_c[i]+1)%dataEMG[i].size();

    }
}



void MainWindow::drawing()
{

#ifdef SERIAL
    for(int p_ind=0;p_ind<2;p_ind++)
    {
        curveTest[p_ind]->signalDrawing();
        curveFeature1[p_ind]->signalDrawing();
        curveFeature2[p_ind]->signalDrawing();
        curveFeature3[p_ind]->signalDrawing();
        curveFeature4[p_ind]->signalDrawing();
    }
    featureOut=SO->featureOut;
#else
    for(int p_ind=0;p_ind<8;p_ind++)
    {
        curveTest[p_ind]->signalDrawing();
        curveFeature1[p_ind]->signalDrawing();
    }
#endif
    //    qDebug()<<SO->featureOut[0];
    emit featureOutSignal(featureOut);
}

void MainWindow::getFeature(QVector<float> x)
{
    qDebug()<<x[0];
}

void MainWindow::reconnect(QString s)
{
#ifdef SERIAL
    SO->close();
    SO->init(s);
#endif
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

