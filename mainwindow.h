
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#define SERIAL
#include "QKeyEvent"
#include "kohonenwidget.h"
#include "headers.h"


#include "serialqobj.h"
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    serial_obj* SO;
#ifdef SERIAL
    serial_obj* SO;
    QwtPlot *d_plot[2];
#else
    QwtPlot *d_plot[8];
#endif
    vector<float> featureOut, featurePreOut;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void drawingInit(QwtPlot *d_plot, QString);
private:
    //    Ui::MainWindow *ui;


    void paintEvent(QPaintEvent *e);
public slots:
    void keyPressEvent(QKeyEvent *event);
//   void clicked(int buttonId);
    void drawing();
    void getFeature(vector<float>);
    void reconnect(QString);
    void getEMG();
//    void kickMyo();
    void getCor();
    void buttonClicked(int);
    void buttonReleased(int );
    void serialChoose();
    void setCOM();

signals:
    void featureOutSignal(vector<float>);
    void sended();
};

#endif // MAINWINDOW_H
