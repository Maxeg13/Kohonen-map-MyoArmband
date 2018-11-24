//8642
//7531
#include "mainwindow.h"
#include "headers.h"
#include "stand_dev.h"
#include"receiver.h"

//SERIAL may be defined in mainwindow.h

#include "stand_dev.h"
//#include "pca.h"


#include <QtNetwork>
#include "drawing.h"
#include "mainwindow.h"
//#include "layer_koh.h"
using namespace std;


//PCA myPCA(1000,16);

int axisScale=1000;
int bufShowSize=300;
int  y_scale=700;

myCurve *curveTest[8], *curveFeature1[8], *curveFeature2[8];




QTimer *timer;
QTimer *timerMyo;
QPainter *painter;
QUdpSocket* socket_m;
QTimer* timerUDP;
Receiver* REC;

vector < vector<float>> dataEMG;
vector < vector < vector<float>>> featureEMG;
int ind_c[8];
int dim_in=4,dim_out=4;

int getInt(vector<uint8_t>& xi, int k);

void MainWindow::connectMyo()
{

}

void MainWindow::sendUDP()
{
    //    static int cnt;
    //    cnt++;
    //    QByteArray ar;
    //    ar.push_back(prepare_send(degToRad(angles[0]=120*((sin(cnt*0.001)+1)/2))));
    //    ar.push_back(prepare_send(degToRad(angles[1]=160*((sin(cnt*0.001)+1)/2)+20)));
    //    ar.push_back(prepare_send(degToRad(angles[2]=50*((cos(cnt*0.001)+1)/2))));
    //    socket_m->writeDatagram(ar,QHostAddress::LocalHost,49123);

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    timerUDP=new QTimer();
    timerUDP->start(2);
    //    timerUDP->setInterval(2);
    socket_m=new QUdpSocket(this);
    //    connect(timerUDP,SIGNAL(timeout()),this,SLOT(sendUDP()));


    featurePreOut.resize(dim_in);
    for (int i=0;i<featurePreOut.size();i++)
        featurePreOut[i]=1;
    featureOut=featurePreOut;
    featureOut.resize(dim_out);


    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);


    int er=100;

    if(er==0)
        emit sign_close();


    dataEMG.resize(8);
    featureEMG.resize(8);

    for( int i_pl=0;i_pl<8;i_pl++)
    {
        featureEMG[i_pl].resize(2);

        d_plot[i_pl] = new QwtPlot(this);
        drawingInit(d_plot[i_pl]);
        d_plot[i_pl]->setAxisScale(QwtPlot::yLeft,-y_scale,y_scale);
        d_plot[i_pl]->setAxisScale(QwtPlot::xBottom,0,bufShowSize);
        //        d_plot[i_pl]->setAxisScaleDiv(QwtPlot::xBottom,QwtScaleDiv::NoTick);
        //        d_plot[i_pl]->setAxisScaleDiv(1,QwtScaleDiv(-10000,10000));




        GL->addWidget(d_plot[i_pl],(i_pl)/4,(i_pl)%4);


        curveTest[i_pl]=new myCurve(bufShowSize, dataEMG[i_pl],d_plot[i_pl],"EMG",Qt::black,Qt::black,ind_c[i_pl]);
        curveFeature1[i_pl]=
                new myCurve(bufShowSize,featureEMG[i_pl][0],d_plot[i_pl],"bipolar feature1",Qt::green,Qt::black,ind_c[i_pl]);
        curveFeature2[i_pl]=
                new myCurve(bufShowSize,featureEMG[i_pl][1],d_plot[i_pl],"bipolar feature1",Qt::red,Qt::black,ind_c[i_pl]);

    }



    setCentralWidget(centralWidget1);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    timer->start(40);


    timerMyo = new QTimer(this);
    connect(timerMyo, SIGNAL(timeout()), this, SLOT(kickMyo()));
    timerMyo->start(2);
    /////////////////
    //    connect(&(collector->qdc),SIGNAL(EMG( vector<float>)),this,SLOT(getEMG( vector<uint8_t>)));


    REC=new Receiver();
    connect(REC,SIGNAL(sig_out(vector<uint8_t>)),this,SLOT(getEMG(vector<uint8_t>)));

}



void MainWindow::kickMyo()
{

    //    static int cnt;
    //    cnt++;
    //    qDebug()<<cnt;
    //    QByteArray ar;
    //    ar.push_back(prepare_send(degToRad(120*((sin(cnt*0.001)+1)/2))));
    //    ar.push_back(prepare_send(degToRad(160*((sin(cnt*0.001)+1)/2)+20)));
    //    ar.push_back(prepare_send(degToRad(50*((cos(cnt*0.001)+1)/2))));
    //    //    qDebug()<<b1;
    //    //    srdSocket->writeDatagram(ar,QHostAddress::LocalHost,srdClientPort.toInt());
    //    socket_m->writeDatagram(ar,QHostAddress::LocalHost,49123);

    static int presc;
    if(presc==2)
    {

        presc=0;
    }
    presc++;

}

//conne
void MainWindow::getEMG( vector<uint8_t> ix)
{
#ifndef SERIAL
    static uint8_t cnt=0;
    cnt++;
    if(cnt%2==0)
    {
        int dim=4;
        vector<float> x;

        x.resize(dim);
        for(int i=0;i<dim;i++)
        {
            x[i]=getInt(ix,i*4);
            //        if(i==2)
            //            x[i]+=15;
        }




        getFeaturesMyo(x,featurePreOut);
        for (int i=0;i<dim;i++)
        {
            ind_c[i]=(ind_c[i]+1)%dataEMG[i].size();
            dataEMG[i][ind_c[i]]=x[i];
            featureEMG[i][0][ind_c[i]]=featurePreOut[i];
            //        featureEMG[i][1][ind_c[i]]=featurePreOut[8+i];
        }
        //    myPCA.updateBuf(featurePreOut);
        //qDebug()<<featureOut.size();
        //        myPCA.proect(featureOut);

        featureOut=featurePreOut;
        //    featureOut.resize(9);
        //    featureOut[8]=20;

        //    for(int i=0;i<4;i++)
        //    {
        //        featureOut[i]=featurePreOut[i]-featurePreOut[(i+4)];
        //    }
    }
#endif
}



void MainWindow::drawing()
{


    for(int p_ind=0;p_ind<8;p_ind++)
    {
        curveTest[p_ind]->signalDrawing();
        curveFeature1[p_ind]->signalDrawing();
        curveFeature2[p_ind]->signalDrawing();
    }

    emit featureOutSignal(featureOut);
}
void MainWindow::getCor()
{

}

void MainWindow::getFeature( vector<float> x)
{
    //    qDebug()<<x[0];

}

void MainWindow::reconnect(QString s)
{

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


    d_plot->setMinimumSize(200,50);

}

MainWindow::~MainWindow()
{

}


void MainWindow::paintEvent(QPaintEvent *e)
{

}

void MainWindow::recon()
{

}

void MainWindow::closeEvent(QCloseEvent *)
{
    //    qDebug()<<"hey";
    this->close();

    emit sign_close();
}

void MainWindow::slot_close()
{



}


int getInt(vector<uint8_t>& xi, int k)
{
    static int i=0;
    static int out=0;
    out=0;
    for(i=(3+k);i>(-1+k);i--)
    {
        out=(out<<8)+xi[i];
    }
    //    qDebug()<<xi[0]<<" "<<xi[1]<<" "<<xi[2]<<" "<<xi[3];
    return out;
}
