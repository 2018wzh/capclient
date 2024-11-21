// capclient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include "logger.h"
#include "utils.h"
#include "hook.h"
Hook hook;
int main()
{
    bool adm = checkAdm();
    if (!adm)
        return elevateAdm();
    hook.installHook();
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    hook.unInstallHook();
}
