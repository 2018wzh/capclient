// cmd.h

#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <unordered_map>

namespace cmd {
    extern bool exitFlag;
    void start();
    void stop();
    void exit();
    void login();
    void logout();
    void status();
    void test();
    void exec(const std::string& input);

    enum class Type {
        START,
        STOP,
        EXIT,
        LOGIN,
        LOGOUT,
        STATUS,
        TEST,
        UNKNOWN
    };

    Type getCommand(const std::string& input);

    typedef std::function<void()> CommandFunction;
    extern std::unordered_map<Type, CommandFunction> commandMap;
}

