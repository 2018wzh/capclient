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
	void Insert(Event::Journal input);
	void Close(time_t duration);};