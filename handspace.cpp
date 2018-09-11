#include "handspace.h"
#include <QDebug>
#include <QMouseEvent>
#include <Qpainter>
using namespace std;
bool pressed;
float wheel_angle;
vector<vector<vector<float>>> angles;
vector<float> angles_s;
float lk=3;
int l0=46*lk, l1=42*lk, l2=10*lk;
int xx0=(l0+l1+l2)*1.1;
int yy0=(l0+l1)*0.5;
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
    angles.resize((int)(2.1*(l0+l1)));
    for(int i=0;i<((int)(2.1*(l0+l1)));i++)
        angles[i].resize((int)(2.1*(l0+l1)));

    for(int i=0;i<(int)(2.1*(l0+l1));i++)
        for(int j=0;j<(int)(2.1*(l0+l1));j++)
            angles[i][j].resize(2);

    angles_s.resize(3);

    int x, y;
    for(float a=0;a<3.14;a+=0.001)
        for(float b=0;b<3.14;b+=0.003)
        {
            coords(x,y,a,b);
            //            if((x>-1)&&(y>-1))
            {
                //                qDebug()<<x+l0+l1;
                //                qDebug()<<y+l0+l1;
                //                qDebug()<<2*(l0+l1);
                angles[x+l0+l1][y+l0+l1][0]=a;
                angles[x+l0+l1][y+l0+l1][1]=b;
            }
        }

}


void HandSpace::mousePressEvent(QMouseEvent *e)
{
    pressed=1;
    QPointF p=(e->pos())-QPointF(xx0,yy0);

    if((p.x()<(l0+l1))&&(p.y()<(l0+l1)))
        if((p.x()>-(l0+l1))&&(p.y()>-(l0+l1)))
            if(angles[p.x()+l0+l1][p.y()+l0+l1][0]&&angles[p.x()+l0+l1][p.y()+l0+l1][1])
            {
                angles_s=angles[p.x()+l0+l1][p.y()+l0+l1];
                angles_s.push_back(wheel_angle);
            }
    //    qDebug()<<angles_s[0];
    emit sendAngles(angles_s);
    update();
}

void HandSpace::mouseMoveEvent(QMouseEvent *e)
{
    if(pressed)
    {
        QPointF p=(e->pos())-QPointF(xx0,yy0);

        if((p.x()<(l0+l1))&&(p.y()<(l0+l1)))
            if((p.x()>-(l0+l1))&&(p.y()>-(l0+l1)))
                if(angles[p.x()+(l0+l1)][p.y()+(l0+l1)][0]&&angles[p.x()+(l0+l1)][p.y()+(l0+l1)][1])
                {
                    angles_s=angles[p.x()+l0+l1][p.y()+l0+l1];
                    angles_s.push_back(wheel_angle);
                }
        emit sendAngles(angles_s);
        update();

    }
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



    int x,y;
    int xw,yw;
    coords(x,y,angles_s[0],angles_s[1]);
    painter->drawLine(xx0,yy0,cos(angles_s[0])*l0+xx0,sin(angles_s[0])*l0+yy0);
    painter->drawLine(cos(angles_s[0])*l0+xx0,sin(angles_s[0])*l0+yy0,x+xx0,y+yy0);
    painter->drawLine(cos(angles_s[0])*l0+xx0,sin(angles_s[0])*l0+yy0,x+xx0,y+yy0);
    painter->drawLine(x+xx0,y+yy0,x+xx0-l2*cos(angles_s[0]-angles_s[1]-angles_s[2]/2),y+yy0-l2*sin(angles_s[0]-angles_s[1]-angles_s[2]/2));
    painter->drawLine(x+xx0,y+yy0,x+xx0-l2*cos(angles_s[0]-angles_s[1]+angles_s[2]/2),y+yy0-l2*sin(angles_s[0]-angles_s[1]+angles_s[2]/2));

    //    qDebug()<<angles_s[0];
    delete painter;
}

void HandSpace::mouseReleaseEvent(QMouseEvent *e)
{
    pressed=0;
}

void HandSpace::wheelEvent(QWheelEvent *e)
{

    float range=3.14/4;
    wheel_angle+=e->delta()/800.;
    if(wheel_angle<-0)
        wheel_angle=-0;
    else if(wheel_angle>range)
        wheel_angle=range;

    angles_s[2]=(wheel_angle);
    emit sendAngles(angles_s);


        qDebug()<<angles_s[2];
    update();
}


