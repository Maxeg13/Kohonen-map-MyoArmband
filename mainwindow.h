
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#define SERIAL
#include "histogramm.h"
#include "QKeyEvent"
//#include "kohonenwidget.h"
#include "headers.h"
#include "receiver.h"

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    Histogramm hist1, hist2;
    Receiver* REC;
    QwtPlot *d_plot[8];

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
    void getEMG(vector<float>);
    void getCor();
    void buttonClicked(int);
    void buttonReleased(int );
    void serialChoose();
signals:
    void featureOutSignal(vector<float>);
    void sended();
};

#endif // MAINWINDOW_H
