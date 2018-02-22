#ifndef KOHONENQWIDGET_H
#define KOHONENQWIDGET_H
#include"headers.h"
#include "layer_koh.h"
#include "QPushButton"

class KohonenWidget : public QWidget
{
    Q_OBJECT
public:
    bool saving_on;
    int dimension;
    QLineEdit *L_E;
    QLineEdit *L_E_F;
    std::vector<float> featureInp;
    QPushButton *saveB;

    QPushButton *corB;
    QPushButton *rstLearningB;
    layer_koh *LK;
    QTimer *timer;
    std::vector<std::vector<float>> data_learn;
    KohonenWidget(std::vector<float>,QWidget *parent = 0);
    void drawingInit(QwtPlot* d_plot);

signals:
    pushStringS(QString);
    corSignal();
public slots:
    void drawing();
    void refresh(std::vector<float>);
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
