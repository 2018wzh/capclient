#pragma once
#include "events.h"
#include "database.h"
#include <string>
extern std::string g_SessionID;
class Journal{
private:
	DB *db;
	std::string session;
	time_t start;
public:
	Journal();
	void Open();
	void Record(Event::Journal e);
	void Close();
};