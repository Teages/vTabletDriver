// vTabletDriverDll.cpp : 定义 DLL 的导出函数。
//

#include "framework.h"
#include "vTabletDriverDll.h"

//test
DLLEXPORT int sum(int a, int b) {
    return a + b;
}

DLLEXPORT OutputVMultiDigitizer* vMulti_connect() {
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
    return outputer;
}

DLLEXPORT int vMulti_isOpened(OutputVMultiDigitizer* outputer) {
    VMulti* vmulti = outputer->vmulti;
    return vmulti->isOpen;
}

DLLEXPORT int vMulti_updateDigi(OutputVMultiDigitizer* outputer, USHORT x, USHORT y, double pressure, unsigned char buttons) {
    outputer->Set(x, y, pressure, buttons);
    outputer->Write();
    return 1;
}
