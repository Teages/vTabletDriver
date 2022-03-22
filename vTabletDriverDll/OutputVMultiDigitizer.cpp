#include "VMulti.h"
#include "OutputVMultiDigitizer.h"

#define LOG_MODULE "VMultiDigitizer"
#include "Logger.h"


//
// Constructor
//
OutputVMultiDigitizer::OutputVMultiDigitizer(VMulti *vmultiInit) {

	vmulti = vmultiInit;

	// Digitizer VMulti report
	report.reportLength = 8;
	report.buttons = 0;
	report.x = 0;
	report.y = 0;
	report.pressure = 0;

	// XP-Pen
	if(vmulti->type == VMulti::TypeXPPen) {
		report.vmultiId = 0x40;
		report.reportId = 5;
		maxPressure = 2047.0;
	}

	// VEIKK
	else if(vmulti->type == VMulti::TypeVEIKK) {
		report.vmultiId = 0x09;
		report.reportId = 2;
		maxPressure = 8191.0;
	}

}


//
// Destructor
//
OutputVMultiDigitizer::~OutputVMultiDigitizer() {
}


//
// Set output
//
bool OutputVMultiDigitizer::Set(USHORT x, USHORT y, double pressure, unsigned char buttons) {

	report.buttons = buttons | 0x20;
	report.x = x;
	report.y = y;
	report.pressure = (USHORT)round(pressure * maxPressure);
	vmulti->SetReport(&report, sizeof(report));

	return true;
}


//
// Write output
//
bool OutputVMultiDigitizer::Write() {

	// Write report to VMulti device if report has changed
	if(vmulti->HasReportChanged()) {

		// Debug message
		if(logger.IsDebugOutputEnabled()) {
			LOG_DEBUGBUFFER(&report, 10, "Report: ");
		}

		vmulti->WriteReport();
		return true;
	}
	return false;
}


//
// Reset output
//
bool OutputVMultiDigitizer::Reset() {

	// Do not reset when buttons are not pressed
	if(report.buttons == 0) {
		return true;
	}

	report.buttons = 0;
	report.pressure = 0;
	vmulti->SetReport(&report, sizeof(report));
	vmulti->WriteReport();
	return true;
}

