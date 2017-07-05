#ifndef SERIAL_DLL
#define SERIAL_DLL
#include "stdafx.h"
#include "targetver.h"
#include <Windows.h>
class Serial
{
public:
    HANDLE hSerial;
void  InitCOM(LPCTSTR );
char  ReadCOM(bool& );

};
#endif // SERIAL_H
