#include "cmd.h"
#include <unordered_map>
#include "logger.h"
#include "user.h"

std::atomic<bool> running;
std::thread runningThread;

static const std::unordered_map<std::string, cmdType> commandMap = {
        {"/start", cmdType::START},
        {"/stop", cmdType::STOP},
		{"/exit", cmdType::EXIT},
		{"/login", cmdType::LOGIN},
		{"/logout", cmdType::LOGOUT},
		{"/status", cmdType::STATUS}
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
    if (!running.load()) {
        running = true;
        runningThread = std::thread(func);
        Logger::get_instance()->info("Starting Hook Thread");
    }
    else {
        Logger::get_instance()->warn("Hook is already running");
    }
}
void cmd::stop() {
    if (running.load()) {
        running = false;
        if (runningThread.joinable()) {
            // 发送一个空消息以确保 GetMessage 能够返回
            PostThreadMessage(GetThreadId(static_cast<HANDLE>(runningThread.native_handle())), WM_QUIT, 0, 0);
            runningThread.join();
        }
        Logger::get_instance()->info("Hook Stopped");
    }
    else {
        Logger::get_instance()->warn("Hook is not running");
    }
}
void cmd::exit() {
    if (running.load()) {
        running = false;
        if (runningThread.joinable()) {
            PostThreadMessage(GetThreadId(static_cast<HANDLE>(runningThread.native_handle())), WM_QUIT, 0, 0);
            runningThread.join();
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
	Logger::get_instance()->info("Hook is {}", running.load() ? "running" : "stopped");
}