// capclient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include "utils.h"
#include "hook.h"
#include "logger.h"
Hook hook;

#ifdef SERVICE_MODE
SERVICE_STATUS g_ServiceStatus;
SERVICE_STATUS_HANDLE g_ServiceStatusHandle;
void WINAPI ServiceCtrlHandler(DWORD dwCtrl) {
    switch (dwCtrl) {
    case SERVICE_CONTROL_STOP:
        hook.unInstallHook();
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        Logger::get_instance()->info("Service stopped");
        SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        hook.unInstallHook();
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        Logger::get_instance()->info("Service stopped");
        SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
        break;
    default:
        break;
    }
}
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv) {
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwWin32ExitCode = NO_ERROR;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;
    g_ServiceStatus.dwWaitHint = 0;
    g_ServiceStatusHandle = RegisterServiceCtrlHandler(TEXT("CaptureClient"), ServiceCtrlHandler);
    if (g_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0) {
        return;
    }
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
    if (!hook.installHook()) {
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
        return;
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    hook.unInstallHook();
    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
}

#endif // SERVICE_MODE
int main()
{
#ifdef SERVICE_MODE
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {TEXT("CaptureClient"), (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };

    // 调用 StartServiceCtrlDispatcher 启动服务
    if (!StartServiceCtrlDispatcher(ServiceTable)) {
        Logger::get_instance()->critical("Service started failed")
        return 1;
    }
#else
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
#endif // SERVICE_MODE
}

