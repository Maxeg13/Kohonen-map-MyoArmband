#ifndef KOHONENQWIDGET_H
#define KOHONENQWIDGET_H
#include"headers.h"
#include "layer_koh.h"
#include "QPushButton"

class KohonenWidget : public QWidget
{
    Q_OBJECT
public:
QLineEdit *L_E;
    std::vector<float> featureInp;
    QPushButton *learnB1;
    QPushButton *learnB2;
    QPushButton *learnB3;
    QPushButton *learnB4;
    QPushButton *learnB5;
    QPushButton *learnB6;
    QPushButton *rstLearningB;
    layer_koh *LK;
    QTimer *timer;
    std::vector<std::vector<float>> data_learn;
    KohonenWidget(QWidget *parent = 0);

    signals:
    pushStringS(QString);
public slots:
    void drawing();
    void refresh(std::vector<float>);
    void learning_1();
    void learning_2();
    void rst();
    void learning_3();
    void learning_4();
    void learning_5();
    void learning_6();
//    void pushString(QString);
    void pushString();
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // KOHONENQWIDGET_H
