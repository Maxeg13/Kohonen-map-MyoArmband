#include"kohonenwidget.h"
#include"headers.h"
#include"layer_koh.h"
float rad=3.;//3

void norm(std::vector<float>& x)
{
    float energy=0;
    for(int i=0;i<x.size();i++)
        energy+=x[i]*x[i];

    //    for(int i=0;i<x.size();i++)
    //        x[i]=x[i]*200/energy;
}

void KohonenWidget::unsaving()
{

    saving_on=0;
    qDebug()<<data_learn.size();
}

void KohonenWidget::saving()
{
    qDebug()<<data_learn.size();
    data_learn.resize(0);
    saving_on=1;
}


void KohonenWidget::rst()
{
    qDebug()<<rad;
    LK->rst();
    for(int i=0;i<10000;i++)
    {
        LK->learnW(data_learn[rand()%data_learn.size()],rad);
    }
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
    painter->scale(0.2,0.2);
    //    LK->indOfMin(featureInp);
    LK->reform();
    LK->draw(*painter);
    delete painter;

}
void KohonenWidget::getRad()
{
    rad=L_E_F->text().toFloat();
}

void KohonenWidget::refresh(std::vector<float> inp)
{    
    featureInp=inp;
    LK->refresh(inp);

    static int cnt=0;
    cnt++;
    if(cnt==1)
    {
        cnt=0;
        if(saving_on)
            data_learn.push_back(featureInp);
    }
}

KohonenWidget::KohonenWidget(std::vector<float> inp,QWidget *parent):QWidget(parent)
{
    QwtPlot* set_plot;
    set_plot=new QwtPlot();
    set_plot->setMinimumSize(QSize(300,300));
    int ii2=150*1;
    set_plot->setAxisScale(QwtPlot::xBottom,-ii2,ii2);
    set_plot->setAxisScale(QwtPlot::yLeft,-ii2,ii2);
    set_plot->setAxisTitle(QwtPlot::yLeft,"EMG2, mV");
    set_plot->setAxisTitle(QwtPlot::xBottom,"EMG1, mV");
    //    set_plot->set
    set_plot->show();
    drawingInit(set_plot);

    saving_on=0;
    dimension=inp.size();
    L_E=new QLineEdit("COM5");
    L_E_F=new QLineEdit("3");

    saveB=new QPushButton("save patterns");

    corB=new QPushButton("correlation");
    rstLearningB=new QPushButton("rst learning");


    featureInp.resize(dimension,0);



    LK=new layer_koh(featureInp,24);
    //    LK->learnW(data_learn[0]);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    connect(saveB,SIGNAL(pressed()),this,SLOT(saving()));
    connect(saveB,SIGNAL(released()),this,SLOT(unsaving()));

    connect(rstLearningB,SIGNAL(released()),this,SLOT(rst()));
    connect(corB,SIGNAL(released()),this,SLOT(getCor()));
    connect(L_E_F,SIGNAL(editingFinished()),this,SLOT(getRad()));

    timer->start(40);
    update();
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
