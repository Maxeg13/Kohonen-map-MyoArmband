#include "mainwindow.h"
#include "headers.h"
#include "stand_dev.h"
//SERIAL may be defined in mainwindow.h
#ifdef SERIAL
#include "serialqobj.h"
#include <QThread>
#else
#include "datacollector.h"
#include "stand_dev.h"
#include "pca.h"
#endif


#include "drawing.h"
//#include "layer_koh.h"



using namespace std;




#ifdef SERIAL
QThread* thread;
myCurve *curveTest[2], *curveFeature1[2], *curveFeature2[2], *curveFeature3[2], *curveFeature4[2];
int bufShowSize=1500;
int axisScale=10000;
#else
PCA myPCA(1000,8);
int axisScale=1000;
int bufShowSize=300;
DataCollector* collector;
myCurve *curveTest[8], *curveFeature1[8];

#endif


QTimer *timer;
QTimer *timerMyo;
QPainter *painter;


std::vector <std::vector<float>> dataEMG;
std::vector <std::vector <std::vector<float>>> featureEMG;
int ind_c[8];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    featurePreOut.resize(8);
    for (int i=0;i<featurePreOut.size();i++)
        featurePreOut[i]=0;
    featureOut=featurePreOut;

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
        d_plot[i_pl]->setAxisScale(QwtPlot::yLeft,-400,400);
        d_plot[i_pl]->setAxisScale(QwtPlot::xBottom,0.8*bufShowSize,bufShowSize);

        curveTest[i_pl]=new myCurve(bufShowSize, dataEMG[i_pl],d_plot[i_pl],"EMG_1",Qt::black,Qt::black,ind_c[i_pl]);

        curveFeature1[i_pl]=new myCurve(bufShowSize,featureEMG[i_pl][0],d_plot[i_pl],"bipolar feature1",Qt::red,Qt::black,ind_c[i_pl]);

        curveFeature2[i_pl]=new myCurve(bufShowSize, featureEMG[i_pl][1],d_plot[i_pl],"force feature",Qt::green,Qt::black,ind_c[i_pl]);

        curveFeature3[i_pl]=new myCurve(bufShowSize, featureEMG[i_pl][2],d_plot[i_pl],"Willison's feature2",Qt::blue,Qt::black,ind_c[i_pl]);

        curveFeature4[i_pl]=new myCurve(bufShowSize, featureEMG[i_pl][3],d_plot[i_pl],"bipolar feature2",Qt::red,Qt::black,ind_c[i_pl]);
    }

    GL->addWidget(d_plot[0],1,1);
    GL->addWidget(d_plot[1],2,1);


#else

    collector=new DataCollector();


    dataEMG.resize(8);
    featureEMG.resize(8);

    for( int i_pl=0;i_pl<8;i_pl++)
    {
        featureEMG[i_pl].resize(1);

        d_plot[i_pl] = new QwtPlot(this);
        drawingInit(d_plot[i_pl]);
        d_plot[i_pl]->setAxisScale(QwtPlot::yLeft,-400,400);
        d_plot[i_pl]->setAxisScale(QwtPlot::xBottom,0,bufShowSize);
        GL->addWidget(d_plot[i_pl],(i_pl)/4,(i_pl)%4);

        curveTest[i_pl]=new myCurve(bufShowSize, dataEMG[i_pl],d_plot[i_pl],"EMG",Qt::black,Qt::black,ind_c[i_pl]);
        curveFeature1[i_pl]=
                new myCurve(bufShowSize,featureEMG[i_pl][0],d_plot[i_pl],"bipolar feature1",Qt::red,Qt::black,ind_c[i_pl]);
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
    connect(&(collector->qdc),SIGNAL(EMG(std::vector<float>)),this,SLOT(getEMG(std::vector<float>)));
#endif
}


void MainWindow::kickMyo()
{
#ifdef SERIAL

#else
    collector->kick(10);
#endif
}

void MainWindow::getEMG(std::vector<float> x)
{
#ifndef SERIAL
    getFeaturesMyo(x,featurePreOut);
    for (int i=0;i<8;i++)
    {
        ind_c[i]=(ind_c[i]+1)%dataEMG[i].size();
        dataEMG[i][ind_c[i]]=x[i];
        featureEMG[i][0][ind_c[i]]=featurePreOut[i];
    }
    myPCA.updateBuf(featurePreOut);

        myPCA.proect(5,featurePreOut);
        featureOut=featurePreOut;
#endif
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
    emit featureOutSignal(featureOut);
}
void MainWindow::getCor()
{
 #ifndef SERIAL
    myPCA.centr();
    myPCA.getCor();
    myPCA.algorithm();
    myPCA.sort();
#endif
//    myPCA.proect(8,v);
}

void MainWindow::getFeature(std::vector<float> x)
{
    qDebug()<<x[0];
}

void MainWindow::reconnect(QString s)
{
#ifdef SERIAL
    SO->close();
    SO->init(s);
#else
//    delete collector;

//    collector=new DataCollector();
#endif
}


void MainWindow::drawingInit(QwtPlot* d_plot)
{

    //        setCentralWidget(MW);

    //canvas().resize(925,342)
    //    d_plot->canvas()->resize(100,150);
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




    // Включить сетку
    // #include <qwt_plot_grid.h>
    //    QwtPlotGrid *grid = new QwtPlotGrid(); //

    //    grid->setMajorPen(QPen( Qt::gray, 2 )); // цвет линий и толщина
    //    grid->attach( d_plot ); // добавить сетку к полю графика

#ifdef SERIAL
    // Параметры осей координат
    d_plot->setAxisTitle(QwtPlot::yLeft, "EMG, mkV");
    d_plot->setAxisTitle(QwtPlot::xBottom, "time");
    d_plot->insertLegend( new QwtLegend() );
#else
    d_plot->setMinimumSize(90,30);
#endif
}

MainWindow::~MainWindow()
{
    //    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *e)
{

}
