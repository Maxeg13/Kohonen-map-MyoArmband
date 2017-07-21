#include "mainwindow.h"
#include "kohonenwidget.h"
#include <QApplication>
#include <QDebug>
#include <QtGui>
#include <QPushButton>
#include <QCoreApplication>
#include <myo/myo.hpp>
#include <windows.h>



class DataCollector : public myo::DeviceListener
{
public:
    DataCollector()
        : emgSamples()
    {
    }

    // onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
    void onUnpair(myo::Myo* myo, uint64_t timestamp)
    {
        // We've lost a Myo.        // Let's clean up some leftover state.
        emgSamples.fill(0);
    }

    // onEmgData() is called whenever a paired Myo has provided new EMG data, and EMG streaming is enabled.
    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
    {
        //        static uint64_t timestampMin;
        //        static uint64_t timestampRelat;
        //        static uint64_t i=0;
        //        if(i==0)timestampMin=timestamp;
        //        i++;
        //        timestampRelat=timestamp-timestampMin;
        //        qDebug()<<i*1000000./timestampRelat;


        for (int i = 0; i < 8; i++) {
            emgSamples[i] = emg[i];
            emgSamplesD[i] = (double) emg[i];
        }
    }

    // There are other virtual functions in DeviceListener that we could override here, like onAccelerometerData().
    // For this example, the functions overridden above are sufficient.
    void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float> &accel)
    {
        accSamplesD[0] = (double) accel.x();
        accSamplesD[1] = (double) accel.y();
        accSamplesD[2] = (double) accel.z();
        accSamplesD[3] = (double) accel.magnitude();
    }

    void print()
    {
        qDebug() << '\r';
        //                for (size_t i = 0; i < emgSamples.size(); i++)
        //                {
        //                    ostringstream oss;
        //                    oss << static_cast<int>(emgSamples[i]);
        //                    string emgString = oss.str();
        //                    QString qemgString = QString::fromStdString(emgString);
        //                    QString qbuffStr =  QString::fromStdString(std::string(4 - emgString.size(), ' '));
        //                    qDebug() << '[' << qemgString <<  qbuffStr << ']';
        //                }
    }

    std::array<double, 8> returnEMGDouble()
    {
        return  emgSamplesD;
    }

    std::array<double, 4> returnACCDouble()
    {
        return  accSamplesD;
    }
    void plot()
    {

    }

    std::array<int8_t, 8> emgSamples;               // The values of this array is set by onEmgData() above.
    std::array<double, 8> emgSamplesD;               // The values of this array is set by onEmgData() above.
    std::array<double, 4> accSamplesD;
};


myo::Hub hub("com.example.emg-data-sample");
DataCollector collector;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    qDebug()<<"wtf2";




    hub.run(5);
    MainWindow SignalMW;
    SignalMW.resize(QSize(600,300));
    QMainWindow KohonenMW;
    KohonenMW.resize(QSize(590,550));
    KohonenWidget KW;



    QGridLayout* GL=new QGridLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);

    int buttons_n=8;
    GL->addWidget(&KW,1,1,1,buttons_n);
    GL->addWidget(KW.learnB1,2,1);
    GL->addWidget(KW.learnB2,2,2);
    GL->addWidget(KW.learnB3,2,3);
    GL->addWidget(KW.learnB4,2,4);
    GL->addWidget(KW.learnB5,2,5);
    GL->addWidget(KW.learnB6,2,6);
    GL->addWidget(KW.rstLearningB,2,7);
    GL->addWidget(KW.L_E,2,buttons_n);
    //    L_E.setText("COM6");


    KohonenMW.setCentralWidget(centralWidget1);
    QObject::connect(&SignalMW,SIGNAL(featureOutSignal(QVector<float>)),&KW,SLOT(refresh(QVector<float>)));
    //QObject::connect(&KW.learnB1,SIGNAL(released()),&KW,SLOT(learning_1()));
    QObject::connect((KW.L_E),SIGNAL(editingFinished()),&KW,SLOT(pushString()));
    QObject::connect(&KW,SIGNAL(pushStringS(QString)),&SignalMW,SLOT(reconnect(QString)));

    KohonenMW.setWindowTitle("Kohonen hex-top Map");
    SignalMW.setWindowTitle("Myographic signals");

    KohonenMW.show();
    SignalMW.show();
    //    w.kohonenWidget->show();
    return a.exec();
}
