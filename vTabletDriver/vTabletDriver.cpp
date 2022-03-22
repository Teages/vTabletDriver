#include <windows.h>
#include <iostream>

#include "Logger.h"
#include "OutputVMultiDigitizer.h"
#include "VMulti.h"

#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "winusb.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ntdll.lib")

int main() {
    std::cout << "Hello World!\n";
    VMulti* vmulti;
    OutputVMultiDigitizer* outputer;

    // Init vMulti
    // VMulti XP-Pen
    vmulti = new VMulti(VMulti::TypeXPPen);
    // VMulti VEIKK
    if (!vmulti->isOpen) {
        LOG_ERROR("Can't open XP-Pen's VMulti! Trying VEIKK's VMulti!\n");
        delete vmulti;
        vmulti = new VMulti(VMulti::TypeVEIKK);
    } else {
        std::cout << "Found XP-Pen's VMulti.";
    }
    // Not Found
    if (!vmulti->isOpen) {
        LOG_ERROR("Can't open VMulti device!\n\n");
        LOG_ERROR("Possible fixes:\n");
        LOG_ERROR("1) Install VMulti driver\n");
        LOG_ERROR("2) Kill PentabletService.exe (XP Pen driver)\n");
        LOG_ERROR(
            "3) Uninstall other tablet drivers and reinstall VMulti driver\n");
        return 0;
    }

    // Init outputer
    outputer = new OutputVMultiDigitizer(vmulti);

    outputer->Set(25565, 25565, 100, 5);

    outputer->Write();

    outputer->Set(0, 0, 0, 5);

    outputer->Write();

    return 1;
}