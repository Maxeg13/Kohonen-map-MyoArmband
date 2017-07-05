#ifndef SERIAL_DLL
#define SERIAL_DLL
#include "stdafx.h"
#include "targetver.h"
#include <Windows.h>

void SERIAL_DLL InitCOM(HANDLE& ,LPCTSTR );

char SERIAL_DLL ReadCOM(HANDLE& ,bool& );

#endif // SERIAL_H
