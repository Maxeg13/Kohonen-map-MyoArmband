#ifndef KOHONENQWIDGET_H
#define KOHONENQWIDGET_H
#include"headers.h"
#include "layer_koh.h"
#include "QPushButton"

class KohonenWidget : public QWidget
{
    Q_OBJECT
public:
    int dimension;
    QLineEdit *L_E;
    QLineEdit *L_E_F;
    std::vector<float> featureInp;
    QPushButton *learnB1;
    QPushButton *learnB2;
    QPushButton *learnB3;
    QPushButton *learnB4;
    QPushButton *learnB5;
    QPushButton *learnB6;
    QPushButton *learnB7;
    QPushButton *learnB8;
    QPushButton *corB;
    QPushButton *rstLearningB;
    layer_koh *LK;
    QTimer *timer;
    std::vector<std::vector<float>> data_learn;
    KohonenWidget(std::vector<float>,QWidget *parent = 0);


signals:
    pushStringS(QString);
    corSignal();
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
    void learning_7();
    void learning_8();
    //    void pushString(QString);
    void pushString();
    void getCor();
    void getRad();
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // KOHONENQWIDGET_H
