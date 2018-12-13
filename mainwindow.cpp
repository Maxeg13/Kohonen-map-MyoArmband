//placement:
//0010
//1000

//8642
//7531
// samples  [][samples][]
#include <QPushButton>
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
QLineEdit *LE, *cor1_le, *cor2_le, *LE_shift;
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

int gest_ind;
int resize_on;



float* perc_inp;
float* perc_out;
perceptron* perc;
perceptron* perc_Y;
vector<vector<float>> dataTest;
vector<vector<float>> sum;
vector<float> maxV;
vector<deque<vector<float>>> data_l_inp;
vector<vector<float>> data_l_out;

vector <vector<float>> dataEMG;
vector <float> difEMG;
vector<float> percBuf;
vector <vector <vector<float>>> featureEMG;
int ind_c[8], ind_p;
int dim_in=16,dim_out=8;
int perc_dim;
int gestures_N=8;//5+...
int fing_N=5;



void convertFromVec(vector<float>& x,float* y, float scale);

void convertFromVec(vector<deque<float>>& x,float* y, float scale);


void MainWindow::buttonClicked(int j)
{


    //    for(int i=0;i<(gestures_N);i++)
    //        data_l_out[gestures_N][i]=high1;



    //    qDebug()<<j;
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

        data_l_inp[gest_ind].resize(0);

        break;
    case 9:

        maxV.resize(gestures_N);
        sum.resize(gestures_N);


        for(int i=0;i<gestures_N;i++)
        {
            sum[i].resize(data_l_inp[i].size(),0);
            for(int j=0;j<sum[i].size();j++)
                for(int k=0;k<data_l_inp[i][j].size();k++)
                    sum[i][j]+=data_l_inp[i][j][k];
        }
        for(int i=0;i<gestures_N;i++)
        {
            vector<float>::iterator it;
            it= max_element(sum[i].begin(),sum[i].end());
            maxV[i]=sum[i][distance(sum[i].begin(),it)];
            //            qDebug()<<i<<"   "<<maxV[i];
        }

        for( int k=0;k<50000;k++)//150000
        {
            for(int i=0;i<gestures_N;i++)
            {

                perc->learn1(data_l_inp[i][k%data_l_inp[i].size()], data_l_out[i],sum[i][k%data_l_inp[i].size()]/(maxV[i]+.0000001));
                //                qDebug()<<sum[i][k%data_l_inp[i].size()]/(maxV[i]+.0000000001);
            }
            //            perc->learn1(data_l_inp[0][k%data_l_inp[0].size()], data_l_out[0]);
        }
        break;
    case 10:
        perc->reset_w();

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
        for(int i=0;i<dataEMG[cor1_le->text().toInt()].size();i++)
            qDebug()<<((dataEMG[cor1_le->text().toInt()][i]>0)?1:(-1))*dataEMG[cor1_le->text().toInt()][i]<<
                                                                                                             " "<<((dataEMG[cor1_le->text().toInt()][i]>0)?1:(-1))*dataEMG[cor2_le->text().toInt()][i];
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
        for(int i=0;i<dataEMG[cor1_le->text().toInt()].size();i++)
        {
            //            if(dataEMG[cor1_le->text().toInt()][i]>0)
            {
                e1+=((dataEMG[cor1_le->text().toInt()][i]>0)?1:(-1))*(dataEMG[cor1_le->text().toInt()][i]);
                e2+=((dataEMG[cor1_le->text().toInt()][i]>0)?1:(-1))*(dataEMG[cor2_le->text().toInt()][i]);

                ee1+=((dataEMG[cor2_le->text().toInt()][i]>0)?1:(-1))*(dataEMG[cor1_le->text().toInt()][i]);
                ee2+=((dataEMG[cor2_le->text().toInt()][i]>0)?1:(-1))*(dataEMG[cor2_le->text().toInt()][i]);
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
        for(int i=0;i<dataEMG[cor1_le->text().toInt()].size();i++)
        {
            //            if(dataEMG[cor1_le->text().toInt()][i]>0)
            {
                e3+=((dataEMG[cor1_le->text().toInt()][i]>0)?1:(-1))*(dataEMG[cor1_le->text().toInt()][i]);
                e4+=((dataEMG[cor1_le->text().toInt()][i]>0)?1:(-1))*(dataEMG[cor2_le->text().toInt()][i]);

                ee3+=((dataEMG[cor2_le->text().toInt()][i]>0)?1:(-1))*(dataEMG[cor1_le->text().toInt()][i]);
                ee4+=((dataEMG[cor2_le->text().toInt()][i]>0)?1:(-1))*(dataEMG[cor2_le->text().toInt()][i]);
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
    qDebug()<<hist1.N;
    perc_dim=hist1.N2;

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

    cor1_le=new QLineEdit;
    cor1_le->setText("6");
    cor2_le=new QLineEdit;
    cor2_le->setText("7");
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

    for(int i=0;i<(12);i++)
    {
        switch(i)
        {
        case 0:
            button_learn=new QPushButton("save weak");break;
        case 1:
            button_learn=new QPushButton("save 1");break;
        case 2:
            button_learn=new QPushButton("save 2");break;
        case 3:
            button_learn=new QPushButton("save 3");break;
        case 4:
            button_learn=new QPushButton("save 4");break;
        case 5:
            button_learn=new QPushButton("save 5");break;
        case 6:
            button_learn=new QPushButton("save ring");break;
        case 7:
            button_learn=new QPushButton("save fist");break;
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
        GL->addWidget(button_learn,2+i/frame_width,1+i%frame_width);

        //        connect(SO,SIGNAL(dataOut(vector<float>)),this,SLOT(getEMG(vector<float>)));

    }

    //    connect(LE,SIGNAL(editingFinished()),this,SLOT(serialChoose()));

    int frame_width=4;

    //    GL->addWidget(slider_x,2+(gestures_N+3)/frame_width,1+(gestures_N+3)%frame_width);
    //    GL->addWidget(slider_y,2+(gestures_N+4)/frame_width,1+(gestures_N+4)%frame_width);
    //    GL->addWidget(LE,2+(gestures_N+5)/frame_width,1+(gestures_N+5)%frame_width);
    //    GL->addWidget(cor1_le,2+(gestures_N+6)/frame_width,1+(gestures_N+6)%frame_width);
    //    GL->addWidget(cor2_le,2+(gestures_N+7)/frame_width,1+(gestures_N+7)%frame_width);
    //    GL->addWidget(LE_shift,2+(gestures_N+8)/frame_width,1+(gestures_N+8)%frame_width);

    GL->setColumnMinimumWidth(0,400);

    //__________________machine learning
    //    perc_inp=new float[perc_dim];
    //    perc_out=new float [2];



    vector<int> constr;
    constr.push_back(hist1.N2+hist2.N2);
    constr.push_back(5);//10
    //    constr.push_back(8);
//    constr.push_back(fing_N);//output
    perc=new perceptron(constr);
//    perc->lr[1]->two_sub_inLayers=1;
    //    perc_Y=new perceptron(constr);

    featurePreOut.resize(dim_in);
    for (int i=0;i<featurePreOut.size();i++)
        featurePreOut[i]=1;
    featureOut=featurePreOut;
    featureOut.resize(dim_out);


    data_l_inp.resize(gestures_N);

    for(int i=0;i<gestures_N;i++)
        data_l_inp[i].resize(1);

    for(int i=0;i<gestures_N;i++)
        data_l_inp[i][0].resize(hist1.N2+hist2.N2);

    data_l_out.resize(gestures_N);
    for (int i=0;i<gestures_N;i++)
        data_l_out[i].resize(fing_N);

    float med1=.3, med2=.45;
    float high1=.7, high2=.9;

    for(int i=0;i<gestures_N;i++)
        for(int j=0;j<fing_N;j++)
            data_l_out[i][j]=0;

    for(int i=0;i<(fing_N);i++)
        data_l_out[i+1][i]=high2;

    for(int i=0;i<2;i++)
        data_l_out[gestures_N-2][i]=1;

    for(int i=0;i<(fing_N);i++)
        data_l_out[gestures_N-1][i]=1;

    for(int i=0;i<gestures_N;i++)
    {
        for(int j=0;j<fing_N;j++)
            cout<<data_l_out[i][j]<<"     ";
        cout<<"\n";
    }

    dataEMG.resize(8);
    featureEMG.resize(8);

    for( int i_pl=0;i_pl<8;i_pl++)
    {
        featureEMG[i_pl].resize(2);

        d_plot[i_pl] = new QwtPlot(this);
        drawingInit(d_plot[i_pl],QString("myo chan ")+QString::number(i_pl+1));
        d_plot[i_pl]->setAxisScale(QwtPlot::yLeft,-2000,2000);
        d_plot[i_pl]->setAxisScale(QwtPlot::xBottom,0,bufShowSize);
        GL->addWidget(d_plot[i_pl],(i_pl)/4,1+(i_pl)%4);

        curveTest[i_pl]=new myCurve(bufShowSize, dataEMG[i_pl],d_plot[i_pl],"EMG",Qt::black,Qt::black,ind_c[i_pl]);
        curveFeature1[i_pl]=
                new myCurve(bufShowSize,featureEMG[i_pl][0],d_plot[i_pl],"bipolar feature1",Qt::green,Qt::black,ind_c[i_pl]);
        curveFeature2[i_pl]=
                new myCurve(bufShowSize,featureEMG[i_pl][1],d_plot[i_pl],"bipolar feature1",Qt::red,Qt::black,ind_c[i_pl]);


        REC=new EMG_UDP_Receiver();
        connect(REC,SIGNAL(sig_out(vector<float>)),this,SLOT(getEMG(vector<float>)));
    }


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
    setCurve=new myCurve(bufShowSize, percBuf,set_plot,"perc out", Qt::black, QColor(255,0,0,255),ind_p);
    setCurve->setPen(QColor(0,0,0,0));
    QwtSymbol* symbol2 = new QwtSymbol( QwtSymbol::Ellipse,
                                        QBrush(QColor(0,0,0)), QPen( Qt::black, 2 ), QSize( 3, 3 ) );
    setCurve->setSymbol( symbol2 );
}



void MainWindow::getEMG(vector<float> x)
{


    //    int dim=x.size();
    int dim=8;
    int ii3=cor1_le->text().toInt();


    int state=1;
    preproc(x);

    getFeaturesMyo(x);//stupid name

    if(test_on)
        LTR.proect(x,ii3,cor2_le->text().toInt());

    //zerro-filling
    int thr=65;
//    if(test_on)
//        if((fabs(x[0])>thr)&&(fabs(x[1])>thr))
//        {
//            x[0]=x[1]=0;
//        }
    //    getFeaturesKhor(x,featurePreOut, state);

    //    if(state)
    int ii=cor1_le->text().toInt();
    int ii2=cor2_le->text().toInt();
    //    int ii=0;
    ////////////
//    static int rising[8],bufEMG[8],rising_buf[8];
//    if((x[ii]-bufEMG[ii])>0)
//        rising[ii]=1;
//    else
//        rising[ii]=0;

//    if((x[ii2]-bufEMG[ii2])>0)
//        rising[ii2]=1;
//    else
//        rising[ii2]=0;


//if((((rising_buf[ii]-rising[ii])>0)&&((rising_buf[ii2]-rising[ii2]))>0))
    for (int i=0;i<dim;i++)
    {
        ind_c[i]=(ind_c[i]+1)%dataEMG[i].size();
        ind_p=ind_c[0];

        dataEMG[i][ind_c[i]]=x[i];

        float h=x[i];
        if(write_on)
            cout<<h<<"  ";
        //        featureEMG[i][0][ind_c[i]]=featurePreOut[i];
        //        featureEMG[i][1][ind_c[i]]=featurePreOut[8+i];
    }

    //////////
//    bufEMG[ii]=x[ii];
//    rising_buf[ii]=rising[ii];
//    bufEMG[ii2]=x[ii2];
//    rising_buf[ii2]=rising[ii2];


    hist1.increment(x[6],x[7]);
    //    qDebug()<<x[2];
    hist2.increment(x[6],x[3]);
    //    qDebug()<<hist1.a[3][3];
    //    difEMG[ind_c[ii]]=dataEMG[ii][ind_c[ii]]-dataEMG[ii][(ind_c[ii]-1)%dataEMG[0].size()];
    if(write_on)
        cout<<endl;


    static int gg=0;
    if(resize_on)
    {
        gg++;
        if(gg%10==0)
        {
            gg=0;
            vector<float> vv;
            //    vv=(hist1.b);

            vv.resize(0);
            for(int i=0;i<hist1.b.size();i++)
                vv.push_back(hist1.b[i]);
            for(int i=0;i<hist2.b.size();i++)
                vv.push_back(hist2.b[i]);
            data_l_inp[gest_ind].push_back(vv);
            //            data_l_inp[gest_ind].push_back(hist2.b);
        }
    }

}



void MainWindow::drawing()
{

    int x;
    int y;

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
        //        curveFeature1[p_ind]->signalDrawing(EMG_scale);
        //        curveFeature2[p_ind]->signalDrawing(EMG_scale);
    }


    //    QString::


    int ii=cor1_le->text().toInt();
    int ii2=cor2_le->text().toInt();

    if((ii>-1)&(ii<8))
    {
//        if(((rising[ii]>0)||((rising[ii2]))>0))
            setCurve->set_Drawing(dataEMG[ii],dataEMG[ii2],LE_shift->text().toInt(),EMG_scale);
    }



    update();
    emit featureOutSignal(featureOut);
}

void MainWindow::buttonReleased(int x)
{
    if((x>-1)&&(x<gestures_N))
        resize_on=0;
    if(x==11)
        write_on=0;

    //    qDebug()<<data_l_inp[gest_ind].size();
    //        qDebug()<<data_l_inp[gest_ind][0].size();
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
    QPainter* painter=new QPainter(this);
    vector<float> vv;
    //    vv=(hist1.b);

    vv.resize(0);
    for(int i=0;i<hist1.b.size();i++)
        vv.push_back(hist1.b[i]);
    for(int i=0;i<hist2.b.size();i++)
        vv.push_back(hist2.b[i]);

    perc->refresh(vv);
    //    for(int i=0;i<perc->out_size;i++)
    //        qDebug()<<*perc->out[i];
    //    qDebug()<<"\n";

    //                        painter->setBrush(brush);
    //                        painter->drawRect(rect=QRect(i*width,j*width,width,width));
    QRect rect=QRect(0,0,400,400);
    QColor color;

    for(int i=0;i<hist1.N;i++)
        for(int j=0;j<hist1.N;j++)
        {
            color.setRed(255-hist1.a[i][j]);
            color.setGreen(255-hist1.a[i][j]);
            color.setBlue(255-hist1.a[i][j]);

            rect.setX(hist1.grid_out[j]);
            rect.setY(hist1.grid_out[i]);
            rect.setWidth(hist1.width);
            rect.setHeight(hist1.width);
            //                rect.setCoords(hist1.grid_out[j], hist1.grid_out[i],hist1.grid_out[j+1], hist1.grid_out[i+1]);
            //            QRect rect=QRect(hist1.grid_out[j],hist1.grid_out[i],hist1.width,hist1.width);
            QPainterPath path;
            path.addRect(rect);
            //    painter->drawPath(path);
            painter->fillPath(path,color);

        }

    for(int i=0;i<hist2.N;i++)
        for(int j=0;j<hist2.N;j++)
        {
            color.setRed(255-hist2.a[i][j]);
            color.setGreen(255-hist2.a[i][j]);
            color.setBlue(255-hist2.a[i][j]);

            rect.setX(hist2.grid_out[j]);
            rect.setY(hist2.grid_out[i]+hist2.width*hist2.N);
            rect.setWidth(hist2.width);
            rect.setHeight(hist2.width);
            //                rect.setCoords(hist2.grid_out[j], hist2.grid_out[i],hist2.grid_out[j+1], hist2.grid_out[i+1]);
            //            QRect rect=QRect(hist2.grid_out[j],hist2.grid_out[i],hist2.width,hist2.width);
            QPainterPath path;
            path.addRect(rect);
            //    painter->drawPath(path);
            painter->fillPath(path,color);
        }

    color.setRed(0);
    color.setGreen(0);
    color.setBlue(0);

    int ff=0;
    vector<float> vvv;
    vvv.resize(fing_N);
    for(int i=0;i<fing_N;i++)
        vvv[i]=(*perc->out[i]);

    getFeatures_gearbox1(ff,vvv);


    for(int i=0;i<perc->out_size;i++)
    {
        rect.setX(hist1.grid_out[hist1.N-1]+30+(/*perc->out_size-1-*/i)*2*hist1.width);
        rect.setY(/*hist1.grid_out[hist1.N-1]*1.5-*/*perc->out[i]*200);
        rect.setWidth(hist1.width*2);
        rect.setHeight(hist1.width*2);

        QPainterPath path;
        path.addRect(rect);
        //    painter->drawPath(path);
        painter->fillPath(path,color);
    }
    delete painter;
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

int thresh(float x)
{
    if(x>255)
        return 255;
    if(x<0)
        return 0;
    return x;
}
