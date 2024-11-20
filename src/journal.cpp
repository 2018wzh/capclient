#include "journal.h"
#include "utils.h"
#include "logger.h"
#include <iostream>
time_t start;
Journal::Journal(std::string fname) {
	//db = new DB(fname);
	//sessionID = uuidxx::uuid::Generate();
	//time(&start);
	Logger::get_instance()->info("Session started");
	//db->Open(toStr(sessionID), start);
}
void Journal::Record(journalEvent e) {
	Logger::get_instance()->info("[{}] {} {}", toStr(e.type),e.value,e.friendly);
	//db->Insert(e);
}
void Journal::Close() {
	time_t end;
	time(&end);
	//db->Close(end-start);
	delete db;
}