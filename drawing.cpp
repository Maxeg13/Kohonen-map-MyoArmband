#include "drawing.h"





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
