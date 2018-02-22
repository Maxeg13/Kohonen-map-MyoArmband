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

    myCurve(std::vector<float> &dataH, QwtPlot* d_plotH,
                     const QString &title, int& ind_ch );
    void set_Drawing(std::vector<float> &x, std::vector<float> &y, int,float k);
    myCurve(int bufShowSize, std::vector<float> &dataH,QwtPlot* d_plotH,const QString &title,
            const QColor &color, const QColor &colorSymbol,int& ind_ch );
    void signalDrawing();
};



#endif // DRAWING_H
