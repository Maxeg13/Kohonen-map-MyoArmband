#ifndef HANDSPACE_H
#define HANDSPACE_H
#include <vector>
#include <QWidget>
using namespace std;
class HandSpace : public QWidget
{
    Q_OBJECT
public:
    explicit HandSpace(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void paintEvent(QPaintEvent *e);
signals:
    void sendAngles(vector<float>);
public slots:
};

#endif // HANDSPACE_H
