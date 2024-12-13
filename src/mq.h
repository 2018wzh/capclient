#pragma once
#include "journal.h"
#include <thread>
#include "events.h"
#include <atomic>
namespace MQ {
	extern std::atomic<bool> Running;
	extern std::thread Thread;
	void Connect();
	void Send(Event::Journal e);
	void Send(std::string s);
	void Disconnect();
}