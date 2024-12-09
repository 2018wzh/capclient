#pragma once
#include "events.h"
#include "database.h"
#include <string>
class Journal{
private:
	DB *db;
	uuidxx::uuid sessionID;
	time_t start;
public:
	Journal();
	void Open();
	void Record(Event::Journal e);
	void Close();
};