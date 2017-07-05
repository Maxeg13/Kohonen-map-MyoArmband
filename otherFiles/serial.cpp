#include "serial.h"
#include "stdafx.h"
#include <Windows.h>
#include "targetver.h"
void Serial::InitCOM(LPCTSTR sPortName)
{


    hSerial = ::CreateFile(sPortName,GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        //    cout << "getting state error\n";
    }
    dcbSerialParams.BaudRate=CBR_115200;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams))
    {
        //    cout << "error setting serial port state\n";
    }
    char data[] = "Hello from C++";
    DWORD dwSize = sizeof(data);
    DWORD dwBytesWritten;
    LPOVERLAPPED ov;

    BOOL iRet = WriteFile (hSerial,data,dwSize,&dwBytesWritten ,NULL);
}

char Serial::ReadCOM(bool& ON)
{
    DWORD iSize;
    char sReceivedChar;
    ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);

    //      while(iSize==0)
    //           ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);

    if(iSize>0)
        ON=1;
        return sReceivedChar;
}

