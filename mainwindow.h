#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "kohonenwidget.h"
#include "headers.h"


#include "serialqobj.h"


//namespace Ui {
//class MainWindow; class KohonenQWidget;
//}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
#ifdef SERIAL
    serial_obj* SO;
#endif
    QVector<float> featureOut;
    QwtPlot *d_plot[2];
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void drawingInit(QwtPlot *d_plot);
private:
    //    Ui::MainWindow *ui;

    void paintEvent(QPaintEvent *e);
public slots:
    void drawing();
    void getFeature(QVector<float>);
    void reconnect(QString);

signals:
    void featureOutSignal(QVector<float>);
    void sended();
};

#endif // MAINWINDOW_H
