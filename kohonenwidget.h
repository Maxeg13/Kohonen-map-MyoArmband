#ifndef KOHONENQWIDGET_H
#define KOHONENQWIDGET_H
#include"headers.h"
#include "layer_koh.h"
#include "QPushButton"

class KohonenWidget : public QWidget
{
    Q_OBJECT
public:

    QVector<float> featureInp;
    QPushButton *learnB1;
    QPushButton *learnB2;
    QPushButton *learnB3;
    QPushButton *learnB4;
    QPushButton *rstLearningB;
    layer_koh *LK;
    QTimer *timer;
    QVector<QVector<float>> data_learn;
    KohonenWidget(QWidget *parent = 0);
public slots:
    void drawing();
    void refresh(QVector<float>);
    void learning_1();
    void learning_2();
    void rst();
    void learning_3();
    void learning_4();
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // KOHONENQWIDGET_H
