#pragma once
#include "journal.h"
#include <thread>
#include <atomic>
extern std::atomic<bool> mqRunning;
extern std::thread mqThread;
struct mqEvent{};
void mqConnect();
void mqSend(journalEvent e);
void mqSend(std::string s);
void mqDisconnect();
mqEvent mqReceive();