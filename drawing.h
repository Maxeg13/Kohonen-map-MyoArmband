#ifndef DRAWING_H
#define DRAWING_H
#include "headers.h"
//#include"serial.h"
using namespace std;

class myCurve:public QwtPlotCurve
{
public:

    vector<float> &data;
    QwtPlot* d_plot;
    QwtSymbol *symbol;
    int& ind_c;

    myCurve(vector<float> &dataH, QwtPlot* d_plotH,
                     const QString &title, int& ind_ch );
    myCurve(int bufShowSize, vector<float> &dataH,QwtPlot* d_plotH,const QString &title,
            const QColor &color, const QColor &colorSymbol,int& ind_ch );
    void signalDrawing();
    void set_Drawing(vector<float> &x, vector<float> &y, int,float k);
    void set_Drawing();
    void addPoints(float** x, float** y, int s);
    void addPoints(float* x, float* y, int s);
};



#endif // DRAWING_H
