#include "database.h"
#include "utils.h"
#include "logger.h"
#include <iostream>
DB::DB(std::string name) {
	db = new SQLite::Database(name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
}
void DB::Open(uuidxx::uuid id,time_t t) {
	table = toStr(id);
	try {
		db->exec("CREATE TABLE IF NOT EXISTS sessions (uuid TEXT PRIMARY KEY, time INTEGER, duration INTEGER);");
		SQLite::Statement ins2idx(*db, "INSERT INTO sessions (uuid, time) VALUES (?, ?);");
		ins2idx.bind(1, table);
		ins2idx.bind(2, t);
		ins2idx.exec();
		db->exec("CREATE TABLE IF NOT EXISTS '" + table + "' (uuid TEXT PRIMARY KEY, type TEXT, time INTEGER, data TEXT); ");
	}
	catch (const std::exception& e) {
		Logger::get_instance()->critical(e.what());
	}
}
void DB::Insert(journalEvent e) {
	try {
		SQLite::Statement query(*db, "INSERT INTO '" + table + "' (uuid, type, time, data) VALUES (?, ?, ?, ?);");
		query.bind(1, toStr(e.id));
		query.bind(2, toStr(e.type));
		query.bind(3, e.time);
		query.bind(4, e.data);
		query.exec();
		Logger::get_instance()->info("Insert success");
	}
	catch (const std::exception& e) {
		Logger::get_instance()->warn(e.what());
	}
}
void DB::Close(time_t d) {
	try{
		SQLite::Statement query(*db, "UPDATE sessions SET duration = ? WHERE uuid = ?");
		query.bind(2, table);
		query.bind(1, d);
		query.exec();
	}
	catch (const std::exception& e) {
		Logger::get_instance()->error(e.what());
	}
}