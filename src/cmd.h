#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <unordered_map>

extern std::atomic<bool> hookRunning;
extern std::thread hookRunningThread;

enum class cmdType {
    START,
    STOP,
    EXIT,
    LOGIN,
    LOGOUT,
    STATUS,
    TEST,
    UNKNOWN
};

cmdType getCommand(const std::string& input);

namespace cmd {
    void start(std::function<void()> func);
    void stop();
    void exit();
    void login();
    void logout();
    void status();
    void test();
}
