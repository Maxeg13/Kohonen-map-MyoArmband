#ifndef KOHONENQWIDGET_H
#define KOHONENQWIDGET_H
#include"headers.h"
//#include
#include "layer_koh.h"
#include "QPushButton"
#include <QMouseEvent>
using namespace std;

class KohonenWidget : public QWidget
{
    Q_OBJECT
public:
    float scale=0.2;
    bool saving_on;
    int dimension;
    QLineEdit *L_E, *L_E_1, *L_E_2;
    QLineEdit *L_E_F;
    vector<float> featureInp;
    QPushButton *saveB;

    QPushButton *corB;
    QPushButton *rstLearningB;
    layer_koh *LK;
    QTimer *timer;
    vector<vector<float>> data_learn;
    KohonenWidget(vector<float>,QWidget *parent = 0);


signals:
    pushStringS(QString);
    corSignal();
    pushWeight(vector<float>);
public slots:
    void mousePressEvent(QMouseEvent *e);
    void drawing();
    void refresh(vector<float>);
    void saving();
    void unsaving();
    void rst();
    //    void pushString(QString);
    void pushString();
    void getCor();
    void getRad();
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // KOHONENQWIDGET_H
