#include "mainwindow.h"
#include "headers.h"
#include "stand_dev.h"
#include "QSignalMapper"
#include "QSlider"
//SERIAL may be defined in mainwindow.h
#ifdef SERIAL
#include "serialqobj.h"
#include <QThread>
#else
#include "datacollector.h"
#include "stand_dev.h"
#include "pca.h"
#endif

//#define IDLE
#include "drawing.h"
#include "perceptron.h"

#include "serial.h"
Serial hSerial;
QSlider *slider_x;
QSlider *slider_y;
int thresh(float);

int slider_x_val;
bool ON1;
char c1;

using namespace std;
#ifdef SERIAL
QThread* thread;
myCurve *curveTest[2], *curveFeature1[2], *curveFeature2[2], *curveFeature3[2], *curveFeature4[2];
int bufShowSize=1500;
int axisScale=10000;
#else
PCA myPCA(1000,16);
int axisScale=1000;
int bufShowSize=300;
DataCollector* collector;
myCurve *curveTest[8], *curveFeature1[8], *curveFeature2[8], *percCurve;
#endif


QTimer *timer;
QTimer *timerMyo;
QPainter *painter;
QwtPlot* perc_pl;
QPushButton *button_learn;
int gestures_N=9;

float* perc_inp;
float* perc_out;
perceptron* perc_X;
perceptron* perc_Y;
vector<vector<float>> dataTest;
vector<vector<float>> data_l_inp;
vector<vector<float>> data_l_out;

vector <vector<float>> dataEMG;
vector<float> percBuf;
vector <vector <vector<float>>> featureEMG;
int ind_c[8], ind_p;
int dim_in=16,dim_out=8;
int perc_dim=8;

void convertFromVec(vector<float>& x,float* y, float scale)
{
    for(int j=0;j<perc_dim;j++)
    {
        y[j]=x[j]*scale;
    }
}

void MainWindow::buttonClicked(int j)
{

    data_l_out[0][0]=0;
    data_l_out[0][1]=0;

    data_l_out[1][0]=-.2;
    data_l_out[1][1]=0;

    data_l_out[2][0]=.2;
    data_l_out[2][1]=0;

    data_l_out[3][0]=-.7;
    data_l_out[3][1]=0;

    data_l_out[4][0]=.7;
    data_l_out[4][1]=0;

    data_l_out[5][0]=0;
    data_l_out[5][1]=.2;

    data_l_out[6][0]=0;
    data_l_out[6][1]=-.2;

    data_l_out[7][0]=0;
    data_l_out[7][1]=.7;

    data_l_out[8][0]=0;
    data_l_out[8][1]=-.7;

    switch(j)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        data_l_inp[j]=featurePreOut;
        break;
    case 9:
        for( int k=0;k<50000;k++)
            for(int i=0;i<gestures_N;i++)
            {
                convertFromVec(data_l_inp[i],perc_inp, 1/800.);
                //                convertFromVec(data_l_out[i],perc_out, 1);

                perc_X->learn1(perc_inp,data_l_out[i][0]);
                perc_Y->learn1(perc_inp,data_l_out[i][1]);
            }
        break;
    case 10:
        perc_X->reset_w();
        perc_Y->reset_w();
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    slider_x=new QSlider;
    int setV=255;
    slider_x->setRange(0,setV);
    slider_x->setValue(setV/2);
    slider_x->setOrientation(Qt::Horizontal);

    slider_y=new QSlider;
    slider_y->setRange(0,255);
    slider_y->setValue(128);
    slider_y->setOrientation(Qt::Horizontal);

    QString qstr=QString("COM4");
    string str1=qstr.toUtf8().constData();
    wstring str(str1.begin(),str1.end());
    hSerial.InitCOM(str.c_str());

    //____________________BUTTONS
    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);
    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(buttonClicked(int)));

    for(int i=0;i<(gestures_N+2);i++)
    {
        switch(i)
        {
        case 0:
            button_learn=new QPushButton("save weak");break;
        case 1:
            button_learn=new QPushButton("save left");break;
        case 2:
            button_learn=new QPushButton("save right");break;
        case 3:
            button_learn=new QPushButton("save strong left");break;
        case 4:
            button_learn=new QPushButton("save strong right");break;
        case 5:
            button_learn=new QPushButton("save up");break;
        case 6:
            button_learn=new QPushButton("save down");break;
        case 7:
            button_learn=new QPushButton("save strong up");break;
        case 8:
            button_learn=new QPushButton("save strong down");break;
        case 9:
            button_learn=new QPushButton("learn");break;
        case 10:
            button_learn=new QPushButton("reset perc");break;

        }

        connect(button_learn, SIGNAL(clicked()),
                signalMapper,         SLOT(map()));
        signalMapper->setMapping(button_learn, i);


        int frame_width=4;
        GL->addWidget(button_learn,2+i/frame_width,i%frame_width);

    }

    int frame_width=4;
    GL->addWidget(slider_x,2+(gestures_N+3)/frame_width,(gestures_N+3)%frame_width);
    GL->addWidget(slider_y,2+(gestures_N+4)/frame_width,(gestures_N+4)%frame_width);




    //__________________machine learning
    perc_inp=new float[perc_dim];
    perc_out=new float [1];

    data_l_inp.resize(gestures_N);
    for (int i=0;i<gestures_N;i++)
        data_l_inp[i].resize(perc_dim);

    for (int j=0;j<gestures_N;j++)
        for(int i=0;i<data_l_inp[0].size();i++)
            data_l_inp[j][i]=0;

    data_l_out.resize(gestures_N);
    for (int i=0;i<gestures_N;i++)
        data_l_out[i].resize(2);


    vector<int> constr;
    constr.push_back(perc_dim);
    constr.push_back(5);
    constr.push_back(5);
    constr.push_back(1);//output
    perc_X=new perceptron(constr);
    perc_Y=new perceptron(constr);

    featurePreOut.resize(dim_in);
    for (int i=0;i<featurePreOut.size();i++)
        featurePreOut[i]=1;
    featureOut=featurePreOut;
    featureOut.resize(dim_out);
    //    qDebug()<<featureOut[0];



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

#ifndef IDLE
    collector=new DataCollector();
#endif



    dataEMG.resize(8);
    featureEMG.resize(8);

    for( int i_pl=0;i_pl<8;i_pl++)
    {
        featureEMG[i_pl].resize(2);

        d_plot[i_pl] = new QwtPlot(this);
        drawingInit(d_plot[i_pl],QString("myo chan ")+QString::number(i_pl));
        d_plot[i_pl]->setAxisScale(QwtPlot::yLeft,-400,400);
        d_plot[i_pl]->setAxisScale(QwtPlot::xBottom,0,bufShowSize);
        GL->addWidget(d_plot[i_pl],(i_pl)/4,(i_pl)%4);

        curveTest[i_pl]=new myCurve(bufShowSize, dataEMG[i_pl],d_plot[i_pl],"EMG",Qt::black,Qt::black,ind_c[i_pl]);
        curveFeature1[i_pl]=
                new myCurve(bufShowSize,featureEMG[i_pl][0],d_plot[i_pl],"bipolar feature1",Qt::green,Qt::black,ind_c[i_pl]);
        curveFeature2[i_pl]=
                new myCurve(bufShowSize,featureEMG[i_pl][1],d_plot[i_pl],"bipolar feature1",Qt::red,Qt::black,ind_c[i_pl]);

    }

    perc_pl=new QwtPlot(this);
    drawingInit(perc_pl,QString("perc out"));
    //    perc_pl->setAxisScale(QwtPlot::yLeft,-400,400);
    //    perc_pl->setAxisScale(QwtPlot::xBottom,0,bufShowSize);
    percCurve=new myCurve(bufShowSize, percBuf,perc_pl,"perc out", Qt::black, Qt::black,ind_p);
    QwtSymbol* symbol = new QwtSymbol( QwtSymbol::Rect,
                                       QBrush(QColor(0,0,0)), QPen( Qt::black, 2 ), QSize( 7, 7 ) );
    percCurve->setSymbol( symbol );
    GL->addWidget(perc_pl,0,4,2,3,Qt::AlignLeft);
    perc_pl->setMinimumWidth(390);
    perc_pl->setAxisScale(QwtPlot::yLeft,-1.5,1.5);
    perc_pl->setAxisScale(QwtPlot::xBottom,-1.5,1.5);

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


#ifndef IDLE
    connect(&(collector->qdc),SIGNAL(EMG(vector<float>)),this,SLOT(getEMG(vector<float>)));
#endif

#endif
}


void MainWindow::kickMyo()
{
#ifdef SERIAL

#else

#ifndef IDLE
    collector->kick(10);
#endif

#endif
}

void MainWindow::getEMG(vector<float> x)
{
#ifndef SERIAL




    getFeaturesMyo(x,featurePreOut);
    for (int i=0;i<8;i++)
    {
        ind_c[i]=(ind_c[i]+1)%dataEMG[i].size();
        ind_p=ind_c[0];
        dataEMG[i][ind_c[i]]=x[i];
        featureEMG[i][0][ind_c[i]]=featurePreOut[i];
        featureEMG[i][1][ind_c[i]]=featurePreOut[8+i];
    }

    myPCA.updateBuf(featurePreOut);
    //qDebug()<<featureOut.size();
    myPCA.proect(featureOut);

    convertFromVec(featurePreOut,perc_inp,1/800.);
    perc_X->refresh(perc_inp);
    perc_Y->refresh(perc_inp);
    percBuf[ind_p]=*perc_X->out[0]*500;

#endif
}



void MainWindow::drawing()
{    
    int x;
    int y;
    hSerial.write((char)1);
    hSerial.write((char)(thresh((0.5+*perc_Y->out[0])*255)));
    hSerial.write((char)(thresh((0.7-*perc_X->out[0])*255)));
//    qDebug()<<thresh((0.5+*perc_Y->out[0])*255);
//    hSerial.write((char)((slider_y->value())));
//    hSerial.write((char)((int)(255-slider_x->value())));

//    hSerial.write((char)(((int)sqrt(slider_y->value()/255.))*255));
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
        curveFeature2[p_ind]->signalDrawing();
    }
    percCurve->pointDrawing(*perc_X->out[0],*perc_Y->out[0]);
    //    percCurve->signalDrawing();

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

void MainWindow::getFeature(vector<float> x)
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


void MainWindow::drawingInit(QwtPlot* d_plot, QString title)
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

    QwtText* qwtt=new QwtText(title);
    qwtt->setFont(QFont("Helvetica", 11,QFont::Normal));

    d_plot->setAxisScale(1,-500,500,200);
    d_plot->setTitle( *qwtt ); // заголовок
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
    d_plot->setMinimumSize(150,40);
#endif
}

MainWindow::~MainWindow()
{
    //    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *e)
{

}

int thresh(float x)
{
    if(x>255)
        return 255;
    if(x<0)
        return 0;
    return x;
}
