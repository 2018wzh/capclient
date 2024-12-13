
#include <iostream>
#include <windows.h>
#include <unordered_map>
#include "utils.h"
#include "hook.h"
#include "logger.h"
#include "cmd.h"
#include "config.h"
int main()
{
#ifdef _DEBUG
	Logger::set_log_level(spdlog::level::debug);
#endif
    bool adm = Utils::checkAdm();
    if (!adm)
        return Utils::elevateAdm();
    try {
		Config::Read();
	}
    catch (std::exception& e) {
        Logger::get_instance()->error(e.what());
        return 1;
    }
    std::string command;
    while (!cmd::exitFlag && std::cout << ">" && std::cin >> command)
		cmd::exec(command);
    return 0;
}
