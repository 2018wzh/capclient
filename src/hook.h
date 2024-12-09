#pragma once
#include "journal.h"
#include <thread>
#include <atomic>
namespace Hook
{
    extern std::atomic<bool> Running;
    extern std::thread Thread;
    void ThreadFunc();
    void installHook();
    void unInstallHook();
};

