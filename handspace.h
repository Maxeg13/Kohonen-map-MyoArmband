#ifndef HANDSPACE_H
#define HANDSPACE_H
#include <vector>
#include <QWidget>
#include <Qtimer>

using namespace std;
class HandSpace : public QWidget
{
    Q_OBJECT
public:
    QTimer* timer2;
    explicit HandSpace(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void paintEvent(QPaintEvent *e);
signals:

    void sendAngles(vector<float>);
    void sendCoords(vector<float>);
public slots:
    void getAngles(vector<float>);
    void myTimerEvent();
};

#endif // HANDSPACE_H
