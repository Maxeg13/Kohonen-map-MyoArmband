#include"kohonenwidget.h"
#include"headers.h"
#include"layer_koh.h"

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

void KohonenWidget::rst()
{
    LK->rst();
    for(int i=0;i<(10000-10000%data_learn.size());i++)
    {
        LK->learnW(data_learn[i%data_learn.size()]);
    }
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


void KohonenWidget::refresh(std::vector<float> inp)
{
    featureInp=inp;
    LK->refresh(inp);
}

KohonenWidget::KohonenWidget(QWidget *parent):QWidget(parent)
{
    L_E=new QLineEdit("COM5");

    learnB1=new QPushButton("learn weak");
    learnB2=new QPushButton("learn 1");
    learnB3=new QPushButton("learn 2");
    learnB4=new QPushButton("learn 3");
    learnB5=new QPushButton("learn 4");
    learnB6=new QPushButton("learn 5");
    rstLearningB=new QPushButton("rst learning");

    data_learn.resize(6);
    for(int i=0;i<data_learn.size();i++)
        data_learn[i].resize(8);
    data_learn[0][0]=1;
    data_learn[0][1]=1;
    data_learn[0][2]=1;
    data_learn[0][3]=1;
    data_learn[0][4]=2;
    data_learn[0][5]=1;
    data_learn[0][6]=1;
    data_learn[0][7]=1;
    data_learn[1]=data_learn[0];
    data_learn[2]=data_learn[0];
    data_learn[3]=data_learn[0];
    data_learn[4]=data_learn[0];
    data_learn[5]=data_learn[0];

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
    connect(rstLearningB,SIGNAL(released()),this,SLOT(rst()));
    timer->start(40);
    update();
}
