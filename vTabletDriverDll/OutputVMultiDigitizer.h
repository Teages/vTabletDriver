#pragma once
#include "VMulti.h"
class OutputVMultiDigitizer {
public:

	// Digitizer vmulti report
	struct {
		BYTE vmultiId;
		BYTE reportLength;
		BYTE reportId;
		BYTE buttons;
		USHORT x;
		USHORT y;
		USHORT pressure;
	} report;

	double maxPressure;

	VMulti *vmulti;

	bool Set(USHORT x, USHORT y, double pressure, unsigned char buttons);
	bool Write();
	bool Reset();

	OutputVMultiDigitizer(VMulti *vmulti);
	~OutputVMultiDigitizer();
};

