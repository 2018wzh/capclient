#include "cmd.h"
#include <unordered_map>
#include "logger.h"
#include "mq.h"
#include "user.h"
#include "hook.h"
#include "config.h"
using namespace cmd;
namespace cmd {
    bool exitFlag = false;
    std::unordered_map<Type, CommandFunction> commandMap = {
        {Type::START, start},
        {Type::STOP, stop},
        {Type::EXIT, exit},
        {Type::LOGIN, login},
        {Type::LOGOUT, logout},
        {Type::STATUS, status},
        {Type::TEST, test}
    };

    Type getCommand(const std::string& input) {
        if (input == "start") return Type::START;
        if (input == "stop") return Type::STOP;
        if (input == "exit") return Type::EXIT;
        if (input == "login") return Type::LOGIN;
        if (input == "logout") return Type::LOGOUT;
        if (input == "status") return Type::STATUS;
        if (input == "test") return Type::TEST;
        return Type::UNKNOWN;
    }

    void exec(const std::string& input) {
        Type commandType = getCommand(input);
        auto it = commandMap.find(commandType);
        if (it != commandMap.end())
            it->second();
        else 
			Logger::get_instance()->error("Unknown command: {}", input);
    }
}


void cmd::start() {
    /*
    if (!currentMQ)
		currentMQ = new MQ(Config::mqServer, Config::mqUser, Config::mqPass);
	*/
    if (!currentUser)
		currentUser = new User(Config::loginToken);
    if (!currentUser->Check()) {
		Logger::get_instance()->warn("Please login first");
        currentUser->Login();
    }
	Hook::Thread = std::thread(Hook::ThreadFunc);
    if (!Hook::Running.load()) {
        Hook::Running = true;
        Logger::get_instance()->info("Starting Hook Thread");
		if (Hook::Thread.joinable())
			Logger::get_instance()->info("Hook Started");
		else
			Logger::get_instance()->error("Hook Start Failed");
    }
    else {
        Logger::get_instance()->warn("Hook is already Running");
    }
    /*
    if (!currentMQ->isRunning())
		currentMQ->Connect();
    else 
        Logger::get_instance()->warn("MQ is already Running");
    */
    if(!MQ::Running)
		MQ::Connect();
	else
		Logger::get_instance()->warn("MQ is already Running");
}
void cmd::stop() {
    if (Hook::Running.load()) {
        Hook::Running = false;
        if (Hook::Thread.joinable()) {
            // 发送一个空消息以确保 GetMessage 能够返回
            PostThreadMessage(GetThreadId(static_cast<HANDLE>(Hook::Thread.native_handle())), WM_QUIT, 0, 0);
            Hook::Thread.join();
        }
        Logger::get_instance()->info("Hook Stopped");
    }
    else {
        Logger::get_instance()->info("Hook is not Running");
    }
    /*
	if (currentMQ->isRunning()) {
		currentMQ->Disconnect();
		Logger::get_instance()->info("MQ Disconnected");
	}
	else {
		Logger::get_instance()->info("MQ is not Running");
	}
    */
	if (MQ::Running) {
		MQ::Disconnect();
		Logger::get_instance()->info("MQ Disconnected");
	}
    else {
        Logger::get_instance()->info("MQ is not Running");
    }
}
void cmd::exit() {
	cmd::stop();
    Config::Save();
    Logger::get_instance()->info("Exiting program");
	exitFlag = true;
}
void cmd::login(){
    if (!currentUser) 
        currentUser = new User(Config::loginToken);
	if (!currentUser->Check())
    	currentUser->Login();
	Logger::get_instance()->info("User {} logged in", currentUser->id);
}
void cmd::logout() {
    cmd::stop();
    try {
        currentUser->Logout();
        Logger::get_instance()->info("User {} logged out", currentUser->id);
	}
	catch (const std::exception& e) {
		Logger::get_instance()->error(e.what());
	}
}
void cmd::status() {
	Logger::get_instance()->info("Hook is {}", Hook::Running.load() ? "hookRunning" : "stopped");
	//Logger::get_instance()->info("MQ is {}", currentMQ->isRunning() ? "running" : "stopped");
    Logger::get_instance()->info("MQ is {}", MQ::Running ? "running" : "stopped");
}
#ifdef _DEBUG
void cmd::test() {
    /*
	if (!currentMQ->isRunning())
		currentMQ->Connect();
    currentMQ->Send("hello world");
    */
    if (!MQ::Running)
        MQ::Connect();
	MQ::Send("hello world");
}
#endif