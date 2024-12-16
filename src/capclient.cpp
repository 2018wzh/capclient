#include "utils.h"
#include "hook.h"
#include "logger.h"
#include "cmd.h"
#include "tray.h"
#include "mq.h"
#include "config.h"
#include "user.h"

#include <iostream>
#include <windows.h>
#include <unordered_map>

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
    Logger::set_log_level(spdlog::level::debug);
#endif
    bool adm = Utils::checkAdm();
    if (!adm)
        return Utils::elevateAdm();
    try
    {
        Config::Read();
    }
    catch (std::exception &e)
    {
        Logger::get_instance()->error(e.what());
        return 1;
    }

    MQ::Connect();
    currentUser = new User;
    Tray tray(hInstance);
    try
    {
        tray.Init();
    }
    catch (std::exception &e)
    {
        Logger::get_instance()->error(e.what());
        return 1;
    }

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
    MQ::Disconnect();
}
