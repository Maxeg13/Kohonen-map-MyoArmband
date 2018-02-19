#include "handwindow.h"
bool mouse_move_on[3];

void HandWindow::loop()
{
    update();
}

HandWindow::HandWindow(QWidget *parent) :
    QMainWindow(parent)
{
    hand=Robohand();
    setMinimumSize(500,500);
    handData.resize(3);
    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->start(40);
    this->update();
    //    emit featureOutSignal(handData);
}

void HandWindow::paintEvent(QPaintEvent *e)
{
    QPainter* painter=new QPainter(this);
    QPen pen=QPen(QBrush(QColor(0,0,0)),5,Qt::SolidLine, Qt::RoundCap);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing, 1);
    hand.get_bones();

    for(int k=0;k<3;k++)
        for(int i=0;i<4;i++)
            painter->drawLine(hand.origin[k]+hand.bones[k][i],hand.origin[k]+hand.bones[k][(i+1)%4]);
    //    QRect rect = QRect(290, 20, 70, 40);
    //    painter->drawRect(rect);
    delete painter;
}
void HandWindow::mousePressEvent(QMouseEvent *e)
{
    //    qDebug()<<e->pos();
    for(int i=0;i<3;i++)
    {
        QPoint a=(e->pos()-hand.origin[1+i]);
        if(QPoint::dotProduct(a,a)<120)
        {
            mouse_move_on[i]=1;
        }
    }
}

void HandWindow::mouseMoveEvent(QMouseEvent *e)
{
    QPoint b;
    for(int i=0;i<3;i++)
        if(mouse_move_on[i])
        {
            switch(i)
            {
            case 0:
                b=(e->pos()-hand.origin[0]);
                b.setY(-b.ry());
                hand.data[0]=((b.rx()<0)?(-1):(1))*acos(b.ry()/sqrt(QPoint::dotProduct(b,b)))/hand.k;
                if((b.rx()<0)&&(b.ry()<0))
                    hand.data[i]=hand.max[i];
                break;
            case 1:
                b=(e->pos()-hand.origin[1]);
                b.setY(-b.ry());
                hand.data[1]=((b.rx()<0)?(-1):(1))*
                        (acos(b.ry()/sqrt(QPoint::dotProduct(b,b))))/hand.k+180-hand.data[0];
                if((b.rx()<0)&&(b.ry()<0))
                    hand.data[i]=hand.max[i];
                break;
            case 2:
                b=(e->pos()-hand.origin[2]);
                b.setY(-b.ry());
                hand.data[2]=((b.rx()<0)?(-1):(1))*
                        (acos(b.ry()/sqrt(QPoint::dotProduct(b,b))))/hand.k+360-hand.data[0]-hand.data[1];
                if((b.rx()<0)&&(b.ry()<0))
                    hand.data[i]=hand.max[i];
            }

        }
    vector<float> h(hand.data,hand.data+2);
    emit  featureOutSignal(h);
}

void HandWindow::mouseReleaseEvent(QMouseEvent *e)
{
    for(int i=0;i<3;i++)
        mouse_move_on[i]=0;
}
