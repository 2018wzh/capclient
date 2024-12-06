#include "cmd.h"
#include <unordered_map>
#include "logger.h"
#include "mq.h"
#include "user.h"

std::atomic<bool> hookRunning;
std::thread hookThread;

static const std::unordered_map<std::string, cmdType> commandMap = {
        {"/start", cmdType::START},
        {"/stop", cmdType::STOP},
		{"/exit", cmdType::EXIT},
		{"/login", cmdType::LOGIN},
		{"/logout", cmdType::LOGOUT},
		{"/status", cmdType::STATUS},
        {"/test", cmdType::TEST}
};
cmdType getCommand(const std::string& input){
    auto it = commandMap.find(input);
    if (it != commandMap.end()) {
        return it->second;
    }
    return cmdType::UNKNOWN;
}
void cmd::start(std::function<void()> func) {
    if (!checkLogin()) {
		Logger::get_instance()->warn("Please login first");
        userLogin();
    }
    if (!hookRunning.load()) {
        hookRunning = true;
        hookThread = std::thread(func);
        Logger::get_instance()->info("Starting Hook Thread");
    }
    else {
        Logger::get_instance()->warn("Hook is already Running");
    }
    if (!mqRunning.load()) 
		mqConnect();
    else 
        Logger::get_instance()->warn("MQ is already Running");
}
void cmd::stop() {
    if (hookRunning.load()) {
        hookRunning = false;
        if (hookThread.joinable()) {
            // 发送一个空消息以确保 GetMessage 能够返回
            PostThreadMessage(GetThreadId(static_cast<HANDLE>(hookThread.native_handle())), WM_QUIT, 0, 0);
            hookThread.join();
        }
        Logger::get_instance()->info("Hook Stopped");
    }
    else {
        Logger::get_instance()->warn("Hook is not Running");
    }
	if (mqRunning.load()) {
		mqDisconnect();
		Logger::get_instance()->info("MQ Disconnected");
	}
	else {
		Logger::get_instance()->warn("MQ is not Running");
	}
}
void cmd::exit() {
    if (hookRunning.load()) {
        hookRunning = false;
        if (hookThread.joinable()) {
            PostThreadMessage(GetThreadId(static_cast<HANDLE>(hookThread.native_handle())), WM_QUIT, 0, 0);
            hookThread.join();
        }
        Logger::get_instance()->info("Hook Stopped");
    }
    Logger::get_instance()->info("Exiting program");

}
void cmd::login(){
	userLogin();

	Logger::get_instance()->info("User {} logged in", getUser().id);
}
void cmd::logout() {
    cmd::stop();
    try {
        Logger::get_instance()->info("User {} logged out", getUser().id);
        userLogout();
	}
	catch (const std::exception& e) {
		Logger::get_instance()->error(e.what());
	}
}
void cmd::status() {
	Logger::get_instance()->info("Hook is {}", hookRunning.load() ? "hookRunning" : "stopped");
	Logger::get_instance()->info("MQ is {}", mqRunning.load() ? "running" : "stopped");
}
void cmd::test() {
	if (!mqRunning.load())
		mqConnect();
    mqSend("hello world");
}