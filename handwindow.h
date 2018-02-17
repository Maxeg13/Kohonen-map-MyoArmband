#ifndef HANDWINDOW_H
#define HANDWINDOW_H
#include "robohand.h"
#include "headers.h"
#include <QTimer>
#include <QMouseEvent>
using namespace std;
class HandWindow : public QMainWindow
{
    Q_OBJECT
public:
    Robohand hand;
    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    QTimer* timer;
    vector<float> handData;
    explicit HandWindow(QWidget *parent = 0);
private:
    void paintEvent(QPaintEvent *e);
public slots:
    void loop();
signals:
    void featureOutSignal(std::vector<float>);
};
#endif // HANDWINDOW_H
