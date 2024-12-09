#include "database.h"
#include "utils.h"
#include "logger.h"
#include "user.h"
#include <iostream>
DB::DB(std::string name) {
	db = new SQLite::Database(name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
}
void DB::Open(uuidxx::uuid id,time_t start) {
	table = Utils::toStr(id);
	try {
		db->exec("CREATE TABLE IF NOT EXISTS sessions (uuid TEXT PRIMARY KEY, user TEXT, start INTEGER, end INTEGER);");
		SQLite::Statement ins2idx(*db, "INSERT INTO sessions (uuid, user, start) VALUES (?, ?, ?);");
		ins2idx.bind(1, table);
		ins2idx.bind(2, currentUser->id);
		ins2idx.bind(3, start);
		ins2idx.exec();
		db->exec("CREATE TABLE IF NOT EXISTS '" + table + "' (uuid TEXT PRIMARY KEY, type TEXT, time INTEGER, data TEXT); ");
	}
	catch (const std::exception& e) {
		Logger::get_instance()->critical(e.what());
	}
}
void DB::Insert(Event::Journal e) {
	try {
		SQLite::Statement query(*db, "INSERT INTO '" + table + "' (uuid, type, time, data) VALUES (?, ?, ?, ?);");
		query.bind(1, Utils::toStr(e.id));
		query.bind(2, Utils::toStr(e.type));
		query.bind(3, e.time);
		query.bind(4, e.data);
		query.exec();
		Logger::get_instance()->debug("Insert success");
	}
	catch (const std::exception& e) {
		Logger::get_instance()->warn(e.what());
	}
}
void DB::Close(time_t end) {
	try{
		SQLite::Statement query(*db, "UPDATE sessions SET end = ? WHERE uuid = ?");
		query.bind(2, table);
		query.bind(1, end);
		query.exec();
	}
	catch (const std::exception& e) {
		Logger::get_instance()->error(e.what());
	}
}