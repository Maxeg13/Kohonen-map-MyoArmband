#include "drawing.h"


myCurve::myCurve(std::vector<float> &dataH, QwtPlot* d_plotH,
                 const QString &title, int& ind_ch ):ind_c(ind_ch),data(dataH)
{
    d_plot=d_plotH;
    setTitle(title);
//    setPen(color,2);
}


myCurve::myCurve(int bufShowSize, std::vector<float> &dataH,QwtPlot* d_plotH,const QString &title,
                 const QColor &color, const QColor &colorSymbol,int& ind_ch ):
    data(dataH),ind_c(ind_ch)
{
    d_plot=d_plotH;
    setTitle(title);
    setPen(color,2);


    dataH.resize(bufShowSize);
    for(int i=0;i<dataH.size();i++)
    {
        dataH[i ]=cos(i/4.);
    }
}

void myCurve::signalDrawing()
{
    // Добавить точки на ранее созданную кривую
    QPolygonF points;

    for (int i=0;i<data.size();i++)
    {
        points<<QPointF(i,data[(ind_c+i+1)%data.size()]);
    }
    setSamples( points ); // ассоциировать набор точек с кривой
    attach( d_plot); // отобразить кривую на графике
}


void myCurve::set_Drawing(std::vector<float>& x, std::vector<float>& y, int ii, float k)
{
    // Добавить точки на ранее созданную кривую
    QPolygonF points;

    for(int i=abs(ii);i<(x.size()-abs(ii));i++)
        points<<QPointF(x[i]*k,y[i+ii]*k);

    setSamples( points ); // ассоциировать набор точек с кривой
    attach( d_plot); // отобразить кривую на графике
}
