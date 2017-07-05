#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "kohonenwidget.h"
#include "headers.h"

//#include "serial.h"
//#ifdef UNICODE
//#define SetWindowText SetWindowTextW
//#else
//#define SetWindowText SetWindowTextA
//#endif





class serial_obj : public QObject
{
Q_OBJECT
    public:
    QVector<float> featureOut;
    int ptr;
    serial_obj();
    ~serial_obj();
public slots:
    void doWork();
};






//namespace Ui {
//class MainWindow; class KohonenQWidget;
//}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    serial_obj* SO;
    QVector<float> featureOut;
    QwtPlot *d_plot;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void drawingInit(QwtPlot *d_plot);
private:
//    Ui::MainWindow *ui;

    void paintEvent(QPaintEvent *e);
public slots:
    void drawing();
    void getFeature(QVector<float>);
signals:
    void featureOutSignal(QVector<float>);
    void sended();
};

#endif // MAINWINDOW_H
