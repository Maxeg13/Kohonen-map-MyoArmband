#include "handspace.h"
#include <QDebug>
#include <QMouseEvent>
#include <Qpainter>
using namespace std;
bool pressed;
int wheel_angle;
vector<vector<vector<float>>> angles;
vector<float> angles_s;
float lk=3;
int l0=46*lk, l1=42*lk;
float degToRad1(float x)
{
    return(x/180*3.14);
}

void coords(int& x, int& y, float a, float b)
{
    x=l0*cos(a)-l1*cos(a-b);
    y=l0*sin(a)-l1*sin(a-b);
}

HandSpace::HandSpace(QWidget *parent) : QWidget(parent)
{
    angles.resize(l0+l1);
    for(int i=0;i<l0+l1;i++)
        angles[i].resize(l0+l1);

    for(int i=0;i<l0+l1;i++)
        for(int j=0;j<l0+l1;j++)
            angles[i][j].resize(3);

    angles_s.resize(3);

    int x, y;
    for(float a=0;a<3.14;a+=0.001)
        for(float b=0;b<3.14;b+=0.001)
        {
            coords(x,y,a,b);
            if((x>-1)&&(y>-1))
            {
            angles[x][y][0]=a;
            angles[x][y][1]=b;
            }
        }
     qDebug()<<angles_s[0];
}


void HandSpace::mousePressEvent(QMouseEvent *e)
{
    pressed=1;
    QPointF p=e->pos()+QPointF(0,0);

    angles_s=angles[p.x()][p.y()];
//    qDebug()<<angles_s[0];

    update();
}

void HandSpace::paintEvent(QPaintEvent *e)
{
    static int ii;
    ii++;
    QPainter* painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, 1);
    QPen pen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    //    painter->drawLine(QPoint(0,0),l0*,sin(angles[0])*l0));


    painter->drawLine(0,0,cos(angles_s[0])*l0,sin(angles_s[0])*l0);
    int x,y;
    coords(x,y,angles_s[0],angles_s[1]);
    painter->drawLine(cos(angles_s[0])*l0,sin(angles_s[0])*l0,x,y);
    qDebug()<<angles_s[0];
    delete painter;
}

void HandSpace::mouseReleaseEvent(QMouseEvent *e)
{
    pressed=0;
}

void HandSpace::wheelEvent(QWheelEvent *e)
{

    int range=60;
    wheel_angle+=e->delta()/8;
    if(wheel_angle<-0)
        wheel_angle=-0;
    else if(wheel_angle>range)
        wheel_angle=range;
    //    emit sendAngles(angles);

    //    angles[2]=(wheel_angle);
    qDebug()<<wheel_angle;
    update();
}

void HandSpace::mouseMoveEvent(QMouseEvent *e)
{
    if(pressed)
    {
        QPointF p=e->pos()+QPointF(0,0);

        angles_s=angles[p.x()][p.y()];
        update();
    }
}
