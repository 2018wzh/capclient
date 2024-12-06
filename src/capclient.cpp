

#include <iostream>
#include <windows.h>
#include <unordered_map>
#include "utils.h"
#include "hook.h"
#include "logger.h"
#include "cmd.h"
#include "config.h"


inline void hookThreadFunc() {
    Hook hook;
    try {
        hook.installHook();
        Logger::get_instance()->info("Hook Installed");
    }
    catch (std::exception& e) {
        Logger::get_instance()->error(e.what());
        hookRunning = false;
        return;
    }

    MSG msg;
    while (hookRunning.load() && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    hook.unInstallHook();
    Logger::get_instance()->info("Hook Uninstalled");
}

int main()
{
#ifdef _DEBUG
	Logger::set_log_level(spdlog::level::debug);
#endif
    bool adm = checkAdm();
    if (!adm)
        return elevateAdm();
    try {
		readConfig();
	}
    catch (std::exception& e) {
        Logger::get_instance()->error(e.what());
        return 1;
    }
    std::string command;
    bool exitFlag = false;
    
    while (!exitFlag && std::cout << ">" && std::cin >> command) {
        switch (getCommand(command)) {
        case cmdType::START:
            cmd::start(hookThreadFunc);
            break;
        case cmdType::STOP:
			cmd::stop();
            break;
        case cmdType::EXIT:
            cmd::exit();
            return 0;
            break;
		case cmdType::LOGIN:
			cmd::login();
			break;
        case cmdType::LOGOUT:
			cmd::logout();
			break;
        case cmdType::STATUS:
            cmd::status();
            break;
		case cmdType::TEST:
			cmd::test();
			break;
        default:
            Logger::get_instance()->info("未知命令。使用 /start 启动，/stop 停止，/exit 退出。");
            break;
        }
    }

    return 0;
}
