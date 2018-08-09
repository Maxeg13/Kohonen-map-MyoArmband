#include"kohonenwidget.h"
#include <QtNetwork>
#include"headers.h"
#include"layer_koh.h"
#include "drawing.h"
#include<QMouseEvent>
#include <QFile>
#include <stdio.h>

float rad=3.;//3
bool rst_k=0;
int rst_cnt=0;
myCurve* set_curve;
QwtSymbol* plot_symbol;
QwtPlot* set_plot;
vector<int> b_ind;
int gesture_i=0;
int ind=0;
QUdpSocket* socket;

float degToRad(float x)
{
    return(x/180*3.14);
}
int8_t prepare_send(float x)
{
    return(x*30);
}

void norm( vector<float>& x)
{
    float energy=0;
    for(int i=0;i<x.size();i++)
        energy+=x[i]*x[i];

    //    for(int i=0;i<x.size();i++)
    //        x[i]=x[i]*200/energy;
}


KohonenWidget::KohonenWidget( vector<float> inp,QWidget *parent):QWidget(parent)
{/*cout<<1;*/

    angles.resize(3);
    socket=new QUdpSocket(this);
    QTimer* timer_UDP;
    timer_UDP=new QTimer(this);
    scale=0.2;
    //    QString str("rad 3");
    //    str.remove(0,4);
    //    qDebug()<<str.toFloat();


    set_plot=new QwtPlot();
    set_plot->setMinimumSize(QSize(300,300));
    int ii2=150*1;
    //    set_plot->setAxisScale(QwtPlot::xBottom,-ii2,ii2);
    //    set_plot->setAxisScale(QwtPlot::yLeft,-ii2,ii2);
    set_plot->setAxisTitle(QwtPlot::yLeft,"RMS1");
    set_plot->setAxisTitle(QwtPlot::xBottom,"RMS2");
    //    set_plot->set
    set_plot->show();
    drawingInit(set_plot);
    set_plot->setMinimumSize(400,400);

    vector<float> h2;
    int ind_p;
    set_curve=new myCurve(h2,set_plot,"perc out", ind_p);
    set_curve->setPen(QColor(0,0,0,0));
    plot_symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                 QBrush(QColor(0,0,0)), QPen( Qt::black, 2 ), QSize( 3, 3 ) );

    set_curve->setSymbol( plot_symbol );

    saving_on=0;
    dimension=inp.size();
    L_E=new QLineEdit("COM5");
    L_E_rad=new QLineEdit("rad: 3");
    L_E_ind1=new QLineEdit("0");
    L_E_ind2=new QLineEdit("1");
    L_E_shift1=new QLineEdit("0");
    L_E_shift2=new QLineEdit("0");
    L_E_shift1->setToolTip(QString("rotate by x"));
    L_E_shift2->setToolTip(QString("rotate by y"));
    L_E_ind1->setToolTip(QString("w[]"));
    L_E_ind2->setToolTip(QString("w[]"));

    reconB=new QPushButton("reconnect");
    gestureB=new QPushButton("change gest");
    saveB=new QPushButton("save patterns");
    corB=new QPushButton("correlation");
    rstB=new QPushButton("rst");
    connectB=new QPushButton("connect to Myo");
    learningB=new QPushButton("learn");
    featureInp.resize(dimension,0);
    data_learn.push_back(featureInp);
    gesture_v.push_back(gesture_i);

    LK=new layer_koh(featureInp,24);
    //    LK->learnW(data_learn[0]);

    timer_UDP=new QTimer(this);
    timer_UDP->start(20);
    socket=new QUdpSocket(this);
    connect(timer_UDP,SIGNAL(timeout()),this,SLOT(sendUDP()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    connect(saveB,SIGNAL(pressed()),this,SLOT(saving()));
    //    connect(saveB,SIGNAL(released()),this,SLOT(unsaving()));

    connect(learningB,SIGNAL(released()),this,SLOT(learning()));
    connect(rstB,SIGNAL(released()),this,SLOT(rst()));
    connect(corB,SIGNAL(released()),this,SLOT(getCor()));
    connect(L_E_rad,SIGNAL(editingFinished()),this,SLOT(getRad()));
    connect(connectB,SIGNAL(released()),this,SLOT(killB()));


    timer->start(40);
    update();
}

void KohonenWidget::sendUDP()
{
//    if(angles.size())
    {
    static int cnt;
    cnt+=30;
//    qDebug()<<cnt;
    QByteArray ar;
    //////////________TEST
//    ar.push_back(prepare_send(degToRad(120*((sin(cnt*0.001)+1)/2))));
//    ar.push_back(prepare_send(degToRad(160*((sin(cnt*0.001)+1)/2)+20)));
//    ar.push_back(prepare_send(degToRad(50*((cos(cnt*0.001)+1)/2))));


    ar.push_back(prepare_send(degToRad(angles[0])));
    ar.push_back(prepare_send(degToRad(angles[1])));
    ar.push_back(prepare_send(degToRad(angles[2])));
    socket->writeDatagram(ar,QHostAddress::LocalHost,49123);
    }
}

void KohonenWidget::unsaving()
{

    saving_on=0;
    //    qDebug()<<data_learn.size();
}

void KohonenWidget::getAngles(vector<int> _angles)
{
    angles=_angles;
}

void KohonenWidget::saving()
{
    //       saveB->setChecked(true);
    saving_on=!saving_on;
    if(saving_on)
    {
        saveB->setText(QString("continue..."));
        data_learn.resize(0);
        gesture_v.resize(gesture_i);
    }
    else
    {
        saveB->setText(QString("save patterns"));
        //        qDebug()<<data_learn.size();
    }
}

void KohonenWidget::learning()
{
    float thr=10000;
    int cnt=0;

    for(int i=0;i<(data_learn.size()-cnt);i++)
    {
        float scatter2=0;
        for(int j=0;j<data_learn[0].size();j++)
        {
            scatter2+=data_learn[i][j]*data_learn[i][j];
        }

        if(false) //        if(scatter2>(thr*thr))
        {
            //            qDebug()<<i;
            data_learn.erase(data_learn.begin()+i);
            i--;
            cnt++;

        }
    }

    rst_k=1;
    rst_cnt=0;
}

void KohonenWidget::rst()
{    
    LK->rst();

}

void KohonenWidget::getCor()
{
    emit(corSignal());
}

void KohonenWidget::drawing()
{
    //    LK->refresh(data_learn[0]);

    update();
}



void KohonenWidget::pushString()
{
    emit(pushStringS(L_E->text()));
}

void KohonenWidget::paintEvent(QPaintEvent *e)
{
    QPainter* painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, 1);
    QPen pen(Qt::black, 8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->scale(scale,scale);
    //    LK->indOfMin(featureInp);

    if(rst_k && !saving_on)
    {
        rst_cnt++;
        for(int i=0;i<100;i++)
            LK->learnW(data_learn[rand()%data_learn.size()],rad);
    }
    //    if(rst_cnt==100)
    //        rst_k=0;

    LK->reform();
    LK->draw(*painter);

    delete painter;

    //    plot_symbol->setColor(QColor(255,0,0));
    //    set_plot->setAutoReplot(0);
    plot_symbol->setPen(QPen(QColor(0,0,0),4));
    set_curve->setSymbol(plot_symbol);
    set_curve->addPoints(LK->w[L_E_ind1->text().toInt()],LK->w[L_E_ind2->text().toInt()],LK->N);
    //    set_curve->set_Drawing();

    //    plot_symbol->setPen(QPen(QColor(255,0,0),4));
    //    set_curve->setSymbol(plot_symbol);
    set_curve->addPoints(&featureInp[L_E_ind1->text().toInt()],&featureInp[L_E_ind2->text().toInt()],1);
    set_curve->set_Drawing();
    //    set_plot->setAutoReplot(0);

}



void KohonenWidget::getRad()
{
    rad=L_E_rad->text().remove(0,4).toFloat();
}

void KohonenWidget::refresh( vector<float> inp)
{    
    featureInp=inp;
    LK->refresh(inp);



    static int cnt=0;
    cnt++;
    if(cnt==1)
    {
        cnt=0;
        if(saving_on)
        {
            data_learn.push_back(featureInp);
            //            angles_m.push_back(angles);
            gesture_v.push_back(gesture_i);
        }
    }


}

void KohonenWidget::killB()
{
    connectB->setDisabled(true);
}

void KohonenWidget::mousePressEvent(QMouseEvent *e)
{

    QPointF p=(e->pos())/scale;
    //    qDebug()<<p;
    //    qDebug()<<LK->SR[0][0];
    int ind=0;
    float pd=0;
    float min=1000000000000000;
    for(int i=0;i<LK->N;i++)
    {
        pd=QPointF::dotProduct((p-LK->SR[i].centre),(p-LK->SR[i].centre));
        if(pd<min)
        {
            //            qDebug()<<LK->SR[ind].w[i];
            min=pd;
            ind=i;
            //            qDebug()<<pd;
        }
    }



    QString filename="Data.txt";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        //file.
        QTextStream stream( &file );

        for(int i=0;i<data_learn.size();i++)
        {
            QString str, STR;
            if(data_learn[i].size()==8)
                for(int j=0;j<data_learn[i].size();j++)
                {
                    str.sprintf("%6.2f   ", data_learn[i][j] );
                    STR+=str;


                }
            str.sprintf("%d\n",gesture_v[i]);
            STR+=str;
            //            STR+=QString::number(gesture_i);
            stream << STR;
        }

    }
    file.close();




}


void KohonenWidget::SHIFT()
{
    LK->is=( L_E_shift1->text().toInt());
    LK->ks=( L_E_shift2->text().toInt());
    //    qDebug()<<LK->is;
}


void KohonenWidget::changeGesture()
{
    gesture_i++;
    gesture_i%=5;
    qDebug()<<gesture_i;
}


void KohonenWidget::closeEvent (QCloseEvent *event)
{
    //   qDebug()<<"hello";
}
void KohonenWidget::drawingInit(QwtPlot* d_plot)
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

void KohonenWidget::slot_close()
{
    close();
}
