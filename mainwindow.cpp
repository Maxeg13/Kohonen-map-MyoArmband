#include "mainwindow.h"
#include "headers.h"
#include "stand_dev.h"
#include "QSignalMapper"
#include "QSlider"
#include "QLineEdit"
//HELLO
//SERIAL may be defined in mainwindow.h

//#include "serialqobj.h"
#include <QThread>

//#include "datacollector.h"/
//#include "stand_dev.h"
#include "pca.h"
//#endif

//#define IDLE
#include "drawing.h"
#include "perceptron.h"
#include "deque"
#include "serial.h"
//#include
//serial_obj* SO;
myCurve *setCurve, *emgCurve, *rmsCurve;
Serial hSerial;
QLineEdit *LE, *LE_cor1, *LE_cor2, *LE_shift;
QSlider *slider_x;
QSlider *slider_y;
float EMG_scale=1;
int thresh(float);

int slider_x_val;
bool test_on;
bool ON1;
bool write_on;
char c1;
bool ser_on;

using namespace std;
const float hh[]={1,2};
vector<float> ab(hh,hh+2);
linearTr LTR=linearTr(ab, ab);
PCA myPCA(1000,16);
int axisScale=1000;
int bufShowSize=700;
//DataCollector* collector;
myCurve *curveTest[8], *curveFeature1[8], *curveFeature2[8], *percCurve;


QTimer *timer;
QTimer *timerCOM;
QPainter *painter;
QwtPlot* perc_pl;
QPushButton *button_learn;
int gestures_N=9;
int gest_ind;
int resize_on;



float* perc_inp;
float* perc_out;
perceptron* perc_X;
perceptron* perc_Y;
vector<vector<float>> dataTest;
vector<vector<deque<float>>> data_l_inp;
vector<vector<float>> data_l_out;

vector <vector<float>> dataEMG;
vector <float> difEMG;
vector<float> percBuf;
vector <vector <vector<float>>> featureEMG;
int ind_c[8], ind_p;
int dim_in=16,dim_out=8;
int perc_dim=8;

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

void convertFromVec(vector<float>& x,float* y, float scale)
{
    for(int j=0;j<perc_dim;j++)
    {
        y[j]=x[j]*scale;
    }
}


void convertFromVec(vector<deque<float>>& x,float* y, float scale)
{
    //    qDebug()<<x[0].size();
    static int l;
    l=rand()%x[0].size();
    for(int i=0;i<x.size();i++)
    {
        y[i]=x[i][l]*scale;
    }
}



void MainWindow::buttonClicked(int j)
{
    float med1=.3, med2=.45;
    float high1=.7, high2=.9;

    data_l_out[0][0]=0;
    data_l_out[0][1]=0;

    data_l_out[1][0]=-med1;
    data_l_out[1][1]=0;

    data_l_out[2][0]=med1;
    data_l_out[2][1]=0;

    data_l_out[3][0]=-high1;
    data_l_out[3][1]=0;

    data_l_out[4][0]=high1;
    data_l_out[4][1]=0;

    data_l_out[5][0]=0;
    data_l_out[5][1]=med2;

    data_l_out[6][0]=0;
    data_l_out[6][1]=-med2;

    data_l_out[7][0]=0;
    data_l_out[7][1]=high2;

    data_l_out[8][0]=0;
    data_l_out[8][1]=-high2;

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

        gest_ind=j;
        resize_on=1;
        //        qDebug()<<"hello";


        for(int i=0;i<perc_dim;i++)
        {
            data_l_inp[gest_ind][i].resize(0);
        }
        //        data_l_inp.
        //        data_l_inp.front()[j].push_back(featurePreOut);
        //        data_l_inp.front()[j][0];
        //        vector<float>::resize(0,1);
        break;
    case 9:
        for( int k=0;k<50000;k++)
            for(int i=0;i<gestures_N;i++)
            {
                convertFromVec(data_l_inp[i],perc_inp, 1/800.);
                perc_X->learn1(perc_inp,data_l_out[i][0]);
                perc_Y->learn1(perc_inp,data_l_out[i][1]);
            }
        break;
    case 10:
        perc_X->reset_w();
        perc_Y->reset_w();
        break;
    case 11:
        write_on=1;
        break;
    }

}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    static float e1,e2,ee1,ee2,
            e3 ,e4, ee3 ,ee4;

    if(event->text()=="l")
    {
        for(int i=0;i<dataEMG[LE_cor1->text().toInt()].size();i++)
            qDebug()<<((dataEMG[LE_cor1->text().toInt()][i]>0)?1:(-1))*dataEMG[LE_cor1->text().toInt()][i]<<
                                                                                                             " "<<((dataEMG[LE_cor1->text().toInt()][i]>0)?1:(-1))*dataEMG[LE_cor2->text().toInt()][i];
    }
    if(event->text()=="t")
    {
        test_on=!test_on;
        qDebug()<<"test_on="<<test_on;
    }
    if(event->text()=="e")
    {
        e1=0;e2=0;
        ee1=0;ee2=0;
        for(int i=0;i<dataEMG[LE_cor1->text().toInt()].size();i++)
        {
            //            if(dataEMG[LE_cor1->text().toInt()][i]>0)
            {
                e1+=((dataEMG[LE_cor1->text().toInt()][i]>0)?1:(-1))*(dataEMG[LE_cor1->text().toInt()][i]);
                e2+=((dataEMG[LE_cor1->text().toInt()][i]>0)?1:(-1))*(dataEMG[LE_cor2->text().toInt()][i]);

                ee1+=((dataEMG[LE_cor2->text().toInt()][i]>0)?1:(-1))*(dataEMG[LE_cor1->text().toInt()][i]);
                ee2+=((dataEMG[LE_cor2->text().toInt()][i]>0)?1:(-1))*(dataEMG[LE_cor2->text().toInt()][i]);
            }
        }
        if((ee1*ee1+ee2*ee2)>(e1*e1+e2*e2))
        {
            e1=ee1;e2=ee2;
        }
        //        float l=sqrt(e1*e1+e2*e2);
        qDebug()<<"e1 e2 = "<<e1<<" "<<e2;
    }
    if(event->text()=="w")
    {
        e3=0;e4=0;
        ee3=0;ee4=0;
        for(int i=0;i<dataEMG[LE_cor1->text().toInt()].size();i++)
        {
            //            if(dataEMG[LE_cor1->text().toInt()][i]>0)
            {
                e3+=((dataEMG[LE_cor1->text().toInt()][i]>0)?1:(-1))*(dataEMG[LE_cor1->text().toInt()][i]);
                e4+=((dataEMG[LE_cor1->text().toInt()][i]>0)?1:(-1))*(dataEMG[LE_cor2->text().toInt()][i]);

                ee3+=((dataEMG[LE_cor2->text().toInt()][i]>0)?1:(-1))*(dataEMG[LE_cor1->text().toInt()][i]);
                ee4+=((dataEMG[LE_cor2->text().toInt()][i]>0)?1:(-1))*(dataEMG[LE_cor2->text().toInt()][i]);
            }
        }
        if((ee3*ee3+ee4*ee4)>(e3*e3+e4*e4))
        {
            e3=ee3;e4=ee4;
        }
        qDebug()<<"e3 e4 = "<<e1<<" "<<e2;
        LTR=linearTr(e1,e2,e3,e4);
        LTR.inv();
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    difEMG.resize(bufShowSize);
    LTR.inv();


    slider_x=new QSlider;
    int setV=255;
    slider_x->setRange(0,setV);
    slider_x->setValue(setV/2-25);
    slider_x->setOrientation(Qt::Horizontal);

    slider_y=new QSlider;
    slider_y->setRange(0,255);
    slider_y->setValue(128-20);
    slider_y->setOrientation(Qt::Horizontal);


    LE=new QLineEdit;
    QString qstr=QString("COM5");
    LE->setText(qstr);
    string str1=qstr.toUtf8().constData();
    wstring str(str1.begin(),str1.end());

    ser_on=1;
    hSerial.InitCOM(str.c_str());

    LE_cor1=new QLineEdit;
    LE_cor1->setText("0");
    LE_cor2=new QLineEdit;
    LE_cor2->setText("1");
    LE_shift=new QLineEdit;
    LE_shift->setText("0");

    //____________________BUTTONS
    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);
    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(buttonClicked(int)));

    QSignalMapper* signalMapper2 = new QSignalMapper(this);
    connect(signalMapper2, SIGNAL(mapped(int)),
            this, SLOT(buttonReleased(int)));

    for(int i=0;i<(gestures_N+3);i++)
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
        case 11:
            button_learn=new QPushButton("write to file");break;

        }

        connect(button_learn, SIGNAL(pressed()),
                signalMapper,         SLOT(map()));
        signalMapper->setMapping(button_learn, i);

        connect(button_learn, SIGNAL(released()),
                signalMapper2,         SLOT(map()));
        signalMapper2->setMapping(button_learn, i);

        int frame_width=4;
        GL->addWidget(button_learn,2+i/frame_width,i%frame_width);

        //        connect(SO,SIGNAL(dataOut(vector<float>)),this,SLOT(getEMG(vector<float>)));

    }

    //    connect(LE,SIGNAL(editingFinished()),this,SLOT(serialChoose()));

    int frame_width=4;
    GL->addWidget(slider_x,2+(gestures_N+3)/frame_width,(gestures_N+3)%frame_width);
    GL->addWidget(slider_y,2+(gestures_N+4)/frame_width,(gestures_N+4)%frame_width);
    GL->addWidget(LE,2+(gestures_N+5)/frame_width,(gestures_N+5)%frame_width);
    GL->addWidget(LE_cor1,2+(gestures_N+6)/frame_width,(gestures_N+6)%frame_width);
    GL->addWidget(LE_cor2,2+(gestures_N+7)/frame_width,(gestures_N+7)%frame_width);
    GL->addWidget(LE_shift,2+(gestures_N+8)/frame_width,(gestures_N+8)%frame_width);


    //__________________machine learning
    perc_inp=new float[perc_dim];
    perc_out=new float [1];

    data_l_inp.resize(gestures_N);
    for(int i=0;i<gestures_N;i++)
        data_l_inp[i].resize(perc_dim);

    //    for (int j=0;j<gestures_N;j++)
    //        for(int i=0;i<data_l_inp[0].size();i++)
    //            data_l_inp[j][i]=0;

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





    dataEMG.resize(8);
    featureEMG.resize(8);

    for( int i_pl=0;i_pl<8;i_pl++)
    {
        featureEMG[i_pl].resize(2);

        d_plot[i_pl] = new QwtPlot(this);
        drawingInit(d_plot[i_pl],QString("myo chan ")+QString::number(i_pl+1));
        d_plot[i_pl]->setAxisScale(QwtPlot::yLeft,-2000,2000);
        d_plot[i_pl]->setAxisScale(QwtPlot::xBottom,0,bufShowSize);
        GL->addWidget(d_plot[i_pl],(i_pl)/4,(i_pl)%4);

        curveTest[i_pl]=new myCurve(bufShowSize, dataEMG[i_pl],d_plot[i_pl],"EMG",Qt::black,Qt::black,ind_c[i_pl]);
        curveFeature1[i_pl]=
                new myCurve(bufShowSize,featureEMG[i_pl][0],d_plot[i_pl],"bipolar feature1",Qt::green,Qt::black,ind_c[i_pl]);
        curveFeature2[i_pl]=
                new myCurve(bufShowSize,featureEMG[i_pl][1],d_plot[i_pl],"bipolar feature1",Qt::red,Qt::black,ind_c[i_pl]);


        REC=new Receiver();
        connect(REC,SIGNAL(sig_out(vector<uint8_t>)),this,SLOT(getEMG(vector<uint8_t>)));
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






    setCentralWidget(centralWidget1);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    timer->start(40);



    QwtPlot* set_plot;
    set_plot=new QwtPlot();
    set_plot->setMinimumSize(QSize(600,600));
    int ii2=800;
    set_plot->setAxisScale(QwtPlot::xBottom,-ii2,ii2);
    set_plot->setAxisScale(QwtPlot::yLeft,-ii2,ii2);
    set_plot->setAxisTitle(QwtPlot::yLeft,"EMG2, mV");
    set_plot->setAxisTitle(QwtPlot::xBottom,"EMG1, mV");
    //    set_plot->set
    set_plot->show();



    
    drawingInit(set_plot,"EMG set");
    setCurve=new myCurve(bufShowSize, percBuf,set_plot,"perc out", Qt::black, QColor(0,0,0,0),ind_p);
    setCurve->setPen(QColor(0,0,0,0));
    QwtSymbol* symbol2 = new QwtSymbol( QwtSymbol::Ellipse,
                                        QBrush(QColor(0,0,0)), QPen( Qt::black, 2 ), QSize( 3, 3 ) );
    setCurve->setSymbol( symbol2 );


    QwtPlot* rms_plot=new QwtPlot();
    rms_plot->setAxisScale(QwtPlot::xBottom,0,140);
    rms_plot->setAxisScale(QwtPlot::yLeft,0,140);
    rms_plot->setAxisTitle(QwtPlot::yLeft,"RMS_2");
    rms_plot->setAxisTitle(QwtPlot::xBottom,"RMS_1");
    rms_plot->setMinimumSize(QSize(300,300));
    drawingInit(rms_plot,"root mean square");
    rms_plot->show();

    rmsCurve=new myCurve(bufShowSize, percBuf,rms_plot,"perc out", Qt::black, QColor(0,0,0,0),ind_p);
    rmsCurve->setPen(QColor(0,0,0,0));
    symbol2 = new QwtSymbol( QwtSymbol::Rect,
                             QBrush(QColor(0,0,0)), QPen( Qt::black, 2 ), QSize( 3, 3 ) );
    rmsCurve->setSymbol( symbol2 );



    //        getEMG(SO->doWork());

}



void MainWindow::getEMG(vector<uint8_t> ix)
{
    int dim=2;
    vector<float> x;

    x.resize(dim);
    for(int i=0;i<dim;i++)
    {
        x[i]=getInt(ix,i*4);
    }

    int s=x.size();
    int ii=LE_cor1->text().toInt();


    int state=1;
    getFeaturesKhor(x,featurePreOut, state);

    if(test_on)
        LTR.proect(x,ii,LE_cor2->text().toInt());

    int thr=70;
    if((fabs(x[0])>thr)&&(fabs(x[1])>thr))
        state=!test_on;
    if(state)
        for (int i=0;i<dim;i++)
        {
            ind_c[i]=(ind_c[i]+1)%dataEMG[i].size();
            ind_p=ind_c[0];

            dataEMG[i][ind_c[i]]=x[i];

            float h=x[i];
            if(write_on)
                cout<<h<<"  ";
            featureEMG[i][0][ind_c[i]]=featurePreOut[i];
            //        featureEMG[i][1][ind_c[i]]=featurePreOut[8+i];
        }


    //    difEMG[ind_c[ii]]=dataEMG[ii][ind_c[ii]]-dataEMG[ii][(ind_c[ii]-1)%dataEMG[0].size()];
    if(write_on)
        cout<<endl;


    static int gg=0;
    if(resize_on)
    {
        gg++;
        if(gg%10==0)
        {
            //            qDebug()<<gg;
            gg=0;
            for(int i=0;i<perc_dim;i++)
            {
                data_l_inp[gest_ind][i].push_back(featurePreOut[i]);
            }
        }
    }

}



void MainWindow::drawing()
{
    int x=(thresh((slider_x->value()/255.-*perc_X->out[0]*1)*255));
    int y=(thresh((slider_y->value()/255.+*perc_Y->out[0])*255));
    //    x*=slider_x->value()/125.;
    //    y*=slider_y->value()/125.;

    if(ser_on)
    {
        hSerial.write((char)1);
#ifndef IDLE

        hSerial.write((char)y);
        hSerial.write((char)x);
#else
        //        qDebug()<<thresh((0.5+*perc_Y->out[0])*255);
        hSerial.write((char)((slider_y->value())));
        hSerial.write((char)((int)(255-slider_x->value())));
#endif
    }

    for(int p_ind=0;p_ind<8;p_ind++)
    {
        curveTest[p_ind]->signalDrawing(EMG_scale);
        curveFeature1[p_ind]->signalDrawing(EMG_scale);
        //        curveFeature2[p_ind]->signalDrawing(EMG_scale);
    }
    //    percCurve->pointDrawing(*perc_X->out[0],*perc_Y->out[0]);

    //    QString::

    int ii=LE_cor1->text().toInt();
    int ii2=LE_cor2->text().toInt();
    //    int ii=0;
    if((ii>-1)&(ii<8))
    {
        setCurve->set_Drawing(dataEMG[ii],dataEMG[ii2],LE_shift->text().toInt(),EMG_scale);
        rmsCurve->set_Drawing(featureEMG[ii][0],featureEMG[ii2][0],LE_shift->text().toInt(), EMG_scale);

    }
    //                    setCurve->set_Drawing(dataEMG[ii],difEMG,LE_shift->text().toInt());
    //    percCurve->signalDrawing();

    emit featureOutSignal(featureOut);
}

void MainWindow::buttonReleased(int x)
{
    if((x>-1)&&(x<gestures_N))
        resize_on=0;
    if(x==11)
        write_on=0;

    //    qDebug()<<data_l_inp[gest_ind][0].size();
}


void MainWindow::getCor()
{
    myPCA.centr();
    myPCA.getCor();
    myPCA.algorithm();
    myPCA.sort();
    //    myPCA.proect(8,v);
}

void MainWindow::getFeature(vector<float> x)
{
    qDebug()<<x[0];
}

void MainWindow::reconnect(QString s)
{

}

void MainWindow::serialChoose()
{
    ser_on=1;
    hSerial.close();
    QString qstr=LE->text();
    string str1=qstr.toUtf8().constData();
    wstring str(str1.begin(),str1.end());
    hSerial.InitCOM(str.c_str());

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


    d_plot->setMinimumSize(220,180);

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
