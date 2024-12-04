#include "journal.h"
#include "utils.h"
#include "logger.h"
#include <iostream>
time_t start;
Journal::Journal(std::string fname) {
	db = new DB(fname);
	sessionID = uuidxx::uuid::Generate();
	time(&start);
	Logger::get_instance()->info("[{}] Session started",toStr(sessionID));
	db->Open(toStr(sessionID), start);
}
void Journal::Record(journalEvent e) {
	Logger::get_instance()->info("[{}][{}] {}", e.time,toStr(e.id),toStr(e.type));
	db->Insert(e);
}
void Journal::Close() {
	time_t end;
	time(&end);
	db->Close(end);
	delete db;
}