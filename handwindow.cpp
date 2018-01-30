#include "handwindow.h"

void HandWindow::loop()
{
    update();
}

HandWindow::HandWindow(QWidget *parent) :
    QMainWindow(parent)
{
    handData.resize(3);
//    emit featureOutSignal(handData);
}

void HandWindow::paintEvent(QPaintEvent *e)
{
    QPainter* painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, 1);
    QRect rect = QRect(290, 20, 70, 40);
    painter->drawRect(rect);
    delete painter;
}
