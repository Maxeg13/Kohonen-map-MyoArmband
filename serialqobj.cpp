#include "serialqobj.h"
//#include "stand_dev.h"

//#include "ftt.h"
int stop_bit=1;
int stop_cnt;
bool hear=1;
float time;
vector<float> data;

serial_obj::serial_obj(QString qstr)
{
//    for(int i=0;i<perc_out_dim;i++)
//        cout<<perc_targ[1][i]<<endl;


    std::string str1=qstr.toUtf8().constData();
    std::wstring str(str1.begin(),str1.end());

    hSerial.InitCOM(str.c_str());//was L"COM5"
//    WT=Wavelet();
}

void serial_obj::init(QString qstr)
{
    std::string str1=qstr.toUtf8().constData();
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
vector<float> serial_obj::doWork()
{

//    while(1)
    {
        static int ptr=0;
        bool readVarON;
        readVar=hSerial.ReadCOM(readVarON);
        if(readVarON)
        {
            if((readVar==(255)))
            {
                if(ptr==4)
                {
                    data.resize(8);

//                    emit dataOut(data);
//                    qDebug()<<data.size();
                }
                ptr=0;
                data.resize(0);
            }
            else
            {
            data.push_back((float)readVar);
            ptr++;
            }

        }
        //            qDebug()<<(int8_t)readVar;
    }
    return(data);
}


