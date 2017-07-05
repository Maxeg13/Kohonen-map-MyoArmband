#ifndef DRAWING_H
#define DRAWING_H
#include "headers.h"
#include"serial.h"

class myCurve:public QwtPlotCurve
{
public:

    QVector<QVector<float>> &data;
    QwtPlot* d_plot;
    QwtSymbol *symbol;
    int& ind_c;

    myCurve(int bufShowSize, QVector<QVector<float>> &dataH,QwtPlot* d_plotH,const QString &title,
            const QColor &color, const QColor &colorSymbol,int& ind_ch ):
        data(dataH),ind_c(ind_ch)
    {
        d_plot=d_plotH;
        setTitle(title);
        setPen(color,2);

        dataH.resize(2);
        dataH[0].resize(bufShowSize);
        dataH[1].resize(bufShowSize);
        for(int i=0;i<dataH[0].size();i++)
        {
            dataH[0][i]=(i*1000);
            dataH[1][i]=cos(i/4.);
        }
    }

    void signalDrawing()
    {
        // Добавить точки на ранее созданную кривую
        QPolygonF points;

        for (int i=0;i<data[1].size();i++)
        {

            points<<QPointF(data[0][i],data[1][(ind_c+i+1)%data[1].size()]);
        }
        setSamples( points ); // ассоциировать набор точек с кривой
        attach( d_plot ); // отобразить кривую на графике
    }
};



#endif // DRAWING_H
