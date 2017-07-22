#include "serialqobj.h"

serial_obj::serial_obj(QString qstr,QVector <QVector<float>>& _dataEMG,
                       QVector <QVector <QVector<float>>>& _featureEMG,
                       int* _ind_c):dataEMG(_dataEMG),featureEMG(_featureEMG)
{
    ind_c=_ind_c;
    std::string str1=qstr.toUtf8().constData();
    std::wstring str(str1.begin(),str1.end());

    hSerial.InitCOM(str.c_str());//was L"COM5"
    //    featureOut.resize(3);
    featureOut.resize(8);
    for(int i=0;i<featureOut.size();i++)
        featureOut[i]=1;

}

void serial_obj::init(QString qstr)
{


    std::string str1=qstr.toUtf8().constData();;
    std::wstring str(str1.begin(),str1.end());

    hSerial.InitCOM(str.c_str());//was L"COM5"
    //    featureOut.resize(3);
}

void serial_obj::close()
{
    hSerial.close();
}
serial_obj::~serial_obj()
{};
void serial_obj::doWork()
{

    while(1)
    {

        bool readVarON;
        readVar=(int8_t)hSerial.ReadCOM(readVarON);

        if(readVarON)
        {
            int presc=3;
            gottenVar[1]=gottenVar[0];
            gottenVar[0]=readVar;
            ptr++;
            ptr%=presc;
            if(((gottenVar[0]!=2)&&(gottenVar[0]!=1))&&(ptr==0))
            {
                //alert!!!
                ptr++;
                ptr%=presc;
            }
            else
            {
                if((ptr==1)&&(gottenVar[1]==1))
                {
                    ind_c[0]=(ind_c[0]+1)%dataEMG[0].size();

                    EMG_I=readVar;

                    dataEMG[0][ind_c[0]]=
                            // FBH[0](
                            8*readVar;//);

                    featureOut[0]=featureEMG[0][0][ind_c[0]]=FE1[0](EMG_I)/20;
                    featureOut[1]=featureEMG[0][1][ind_c[0]]=LPF[0](STD[0](EMG_I));
                    featureOut[2]=featureEMG[0][2][ind_c[0]]=LPF[1](WA[0](EMG_I));
                    featureOut[3]=featureEMG[0][3][ind_c[0]]=(400*LPF2[0]((killRange(MFV[0](EMG_I),30))));;
                    //emit(learnSig())
                }
                if((ptr==1)&&(gottenVar[1]==2))
                {
                    ind_c[1]=(ind_c[1]+1)%dataEMG[1].size();

                    EMG_I=readVar;

                    dataEMG[1][ind_c[1]]=
                            // FBH[0](
                            8*readVar;//);

                    featureOut[4]=featureEMG[1][0][ind_c[1]]=FE1[1](EMG_I)/20;
                    featureOut[5]=featureEMG[1][1][ind_c[1]]=LPF[2](STD[1](EMG_I));
                    featureOut[6]=featureEMG[1][2][ind_c[1]]=LPF[3](WA[1](EMG_I));
                    featureOut[7]=featureEMG[1][3][ind_c[1]]=(400*LPF2[1]((killRange(MFV[1](EMG_I),30))));;
                    //emit(learnSig())
                }

            }
        }
    }
}
