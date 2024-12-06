#include "journal.h"
#include "utils.h"
#include "logger.h"
#include "mq.h"
Journal::Journal(std::string fname) {
	time_t start;
	db = new DB(fname);
	sessionID = uuidxx::uuid::Generate();
	time(&start);
	Logger::get_instance()->info("[{}] Session started",toStr(sessionID));
	db->Open(toStr(sessionID), start);
}
void Journal::Record(journalEvent e) {
	Logger::get_instance()->info("[{}][{}] {}", e.time,toStr(e.id),toStr(e.type));
	mqSend(e);
	db->Insert(e);
}
void Journal::Close() {
	time_t end;
	time(&end);
	db->Close(end);
	delete db;
}