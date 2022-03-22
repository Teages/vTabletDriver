#ifdef VTABLETDRIVERDLL_EXPORTS
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

#define DLLEXPORT extern "C" __declspec(dllexport) //放在 #include "stdafx.h" 之后

#include "VMulti.h"
#include "OutputVMultiDigitizer.h"
#include "Logger.h"


DLLEXPORT int sum(int a, int b);

DLLEXPORT OutputVMultiDigitizer* vMulti_connect();

DLLEXPORT int vMulti_isOpened(OutputVMultiDigitizer* outputer);

DLLEXPORT int vMulti_updateDigi(OutputVMultiDigitizer* outputer, USHORT x, USHORT y, double pressure, unsigned char buttons);
