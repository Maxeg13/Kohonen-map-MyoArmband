#include "handwindow.h"

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

    for(int i=0;i<4;i++)
        painter->drawLine(hand.origin[0]+hand.bones[0][i],hand.origin[0]+hand.bones[0][(i+1)%4]);
//    QRect rect = QRect(290, 20, 70, 40);
//    painter->drawRect(rect);
    delete painter;
}
void HandWindow::mousePressEvent(QMouseEvent *e)
{
    qDebug()<<e->pos();
}
