
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#define SERIAL

#include "kohonenwidget.h"
#include "headers.h"


#include "serialqobj.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
//    HandWindow* HW;
#ifdef SERIAL
    serial_obj* SO;
    QwtPlot *d_plot[2];
#else
    QwtPlot *d_plot[8];
#endif
    std::vector<float> featureOut, featurePreOut;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void drawingInit(QwtPlot *d_plot);
private:
    //    Ui::MainWindow *ui;

    void paintEvent(QPaintEvent *e);
public slots:
    void drawing();
    void getFeature(std::vector<float>);
    void reconnect(QString);
    void getEMG(std::vector<float>);
    void kickMyo();
    void getCor();
signals:
    void featureOutSignal(std::vector<float>);
    void sended();
};

#endif // MAINWINDOW_H
