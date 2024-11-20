#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include "events.h"
class DB {
private:
	SQLite::Database *db;
	std::string table;
public:
	DB(std::string name);
	void Open(uuidxx::uuid id,time_t t);
	void Insert(journalEvent input);
	void Close(time_t duration);
	int Callback(void* NotUsed, int argc, char** argv, char** azColName);
};