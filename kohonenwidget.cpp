#include"kohonenwidget.h"
#include"headers.h"
#include"layer_koh.h"
float rad=1.;

void norm(std::vector<float>& x)
{
    float energy=0;
    for(int i=0;i<x.size();i++)
        energy+=x[i]*x[i];

    //    for(int i=0;i<x.size();i++)
    //        x[i]=x[i]*200/energy;
}

void KohonenWidget::learning_1()
{
    int i=0;
    data_learn[i]=featureInp;
    norm( data_learn[i]);
}

void KohonenWidget::learning_2()
{
    int i=1;
    data_learn[i]=featureInp;
    norm( data_learn[i]);
}

void KohonenWidget::learning_3()
{
    int i=2;
    data_learn[i]=featureInp;
    norm( data_learn[i]);
}


void KohonenWidget::learning_4()
{
    int i=3;
    data_learn[i]=featureInp;
    norm( data_learn[i]);
}

void KohonenWidget::learning_5()
{
    int i=4;
    data_learn[i]=featureInp;
    norm( data_learn[i]);
}

void KohonenWidget::learning_6()
{
    int i=5;
    data_learn[i]=featureInp;
    norm( data_learn[i]);
}



void KohonenWidget::learning_7()
{
    int i=6;
    data_learn[i]=featureInp;
    norm( data_learn[i]);
}


void KohonenWidget::learning_8()
{
    int i=7;
    data_learn[i]=featureInp;
    norm( data_learn[i]);
}

void KohonenWidget::rst()
{
    LK->rst();
    for(int i=0;i<(10000-10000%data_learn.size());i++)
    {

        LK->learnW(data_learn[i%data_learn.size()],rad);
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
}

KohonenWidget::KohonenWidget(std::vector<float> inp,QWidget *parent):QWidget(parent)
{
    dimension=inp.size();
    L_E=new QLineEdit("COM5");
    L_E_F=new QLineEdit("1");

    learnB1=new QPushButton("learn weak");
    learnB2=new QPushButton("learn 1");
    learnB3=new QPushButton("learn 2");
    learnB4=new QPushButton("learn 3");
    learnB5=new QPushButton("learn 4");
    learnB6=new QPushButton("learn 5");
    learnB7=new QPushButton("learn 6");
    learnB8=new QPushButton("learn 7");
    corB=new QPushButton("correlation");
    rstLearningB=new QPushButton("rst learning");

    data_learn.resize(8);
    for(int i=0;i<data_learn.size();i++)
        data_learn[i].resize(dimension);
    for(int i=0;i<dimension;i++)
        data_learn[0][i]=1;
    for(int i=1;i<8;i++)
        data_learn[i]=data_learn[0];


    featureInp.resize(data_learn[0].size());
    for(int i=0;i<featureInp.size();i++)
        featureInp[i]=0;


    LK=new layer_koh(data_learn[0],24);
    //    LK->learnW(data_learn[0]);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    connect(learnB1,SIGNAL(released()),this,SLOT(learning_1()));
    connect(learnB2,SIGNAL(released()),this,SLOT(learning_2()));
    connect(learnB3,SIGNAL(released()),this,SLOT(learning_3()));
    connect(learnB4,SIGNAL(released()),this,SLOT(learning_4()));
    connect(learnB5,SIGNAL(released()),this,SLOT(learning_5()));
    connect(learnB6,SIGNAL(released()),this,SLOT(learning_6()));
       connect(learnB7,SIGNAL(released()),this,SLOT(learning_7()));
          connect(learnB8,SIGNAL(released()),this,SLOT(learning_8()));
    connect(rstLearningB,SIGNAL(released()),this,SLOT(rst()));
    connect(corB,SIGNAL(released()),this,SLOT(getCor()));
    connect(L_E_F,SIGNAL(editingFinished()),this,SLOT(getRad()));

    timer->start(40);
    update();
}
