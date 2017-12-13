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

void KohonenWidget::unsaving()
{

    saving_on=0;
    qDebug()<<data_learn.size();
}

void KohonenWidget::saving()
{
    data_learn.resize(0);
    saving_on=1;
    int i=0;
    //    data_learn.push_back(featureInp);
    //    norm( data_learn[i]);
}


void KohonenWidget::rst()
{
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
    if(cnt==2)
    {
        cnt=0;
        if(saving_on)
            data_learn.push_back(featureInp);
    }
}

KohonenWidget::KohonenWidget(std::vector<float> inp,QWidget *parent):QWidget(parent)
{
    saving_on=0;
    dimension=inp.size();
    L_E=new QLineEdit("COM5");
    L_E_F=new QLineEdit("1");

    saveB=new QPushButton("save patterns");

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
    connect(saveB,SIGNAL(clicked()),this,SLOT(saving()));
    connect(saveB,SIGNAL(released()),this,SLOT(unsaving()));

    connect(rstLearningB,SIGNAL(released()),this,SLOT(rst()));
    connect(corB,SIGNAL(released()),this,SLOT(getCor()));
    connect(L_E_F,SIGNAL(editingFinished()),this,SLOT(getRad()));

    timer->start(40);
    update();
}
