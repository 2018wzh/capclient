#pragma once
#include "events.h"
#include "database.h"
#include <string>
class Journal{
private:
	DB *db;
	uuidxx::uuid sessionID;
public:
	Journal(std::string filename);
	void Record(journalEvent e);
	void Close();
};