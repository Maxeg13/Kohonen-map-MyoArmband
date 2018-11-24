#ifndef KOHONENQWIDGET_H
#define KOHONENQWIDGET_H
#include"headers.h"
#include "layer_koh.h"
#include "QPushButton"
using namespace std;

class KohonenWidget : public QWidget
{
    Q_OBJECT
public:
    vector< vector <float>> angles_m;
    vector<float>  angles_out;
    vector<float> angles;

    float scale;
    bool saving_on;
    int dimension;
    QLineEdit *L_E;
    QLineEdit *L_E_rad;
    QLineEdit *L_E_ind1, *L_E_ind2;
    QLineEdit *L_E_shift1, *L_E_shift2;
    vector<float> featureInp;
    QPushButton *saveB;
    QPushButton *corB;
    QPushButton *rstB;
    QPushButton *learningB;
    QPushButton *connectB;
    QPushButton *reconB;
    QPushButton *gestureB;
    layer_koh *LK;
    QTimer *timer;
    vector< vector<float>> data_learn;
    vector<int> gesture_v;
    KohonenWidget( int ,QWidget *parent = 0);
//    ~KohonenWidget();

    void drawingInit(QwtPlot* d_plot);
    void closeEvent (QCloseEvent *event);
    void mousePressEvent(QMouseEvent *);
signals:
    sendAngles(vector<float>);
    pushStringS(QString);
    corSignal();
    sign_close();
public slots:
    void getAngles(vector<float> _angles);

//    void sendUDP();
    void SHIFT();
    void changeGesture();
    void learning();
    void drawing();
    void refresh( vector<float>);
    void saving();
    void unsaving();
    void rst();
    //    void pushString(QString);
    void pushString();
    void getCor();
    void getRad();
    void slot_close();
    void killB();


protected:
    void paintEvent(QPaintEvent *e);
};

#endif // KOHONENQWIDGET_H
