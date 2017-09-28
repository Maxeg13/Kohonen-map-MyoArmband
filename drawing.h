#ifndef DRAWING_H
#define DRAWING_H
#include "headers.h"
//#include"serial.h"

class myCurve:public QwtPlotCurve
{
public:

    std::vector<float> &data;
    QwtPlot* d_plot;
    QwtSymbol *symbol;
    int& ind_c;

    myCurve(int bufShowSize, std::vector<float> &dataH,QwtPlot* d_plotH,const QString &title,
            const QColor &color, const QColor &colorSymbol,int& ind_ch );
    void signalDrawing();
    void pointDrawing(float , float);
};



#endif // DRAWING_H
