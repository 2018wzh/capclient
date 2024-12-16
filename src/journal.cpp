#include "journal.h"
#include "utils.h"
#include "logger.h"
#include "mq.h"
#include "user.h"
#include "config.h"
std::string g_SessionID;
Journal::Journal() {
	db = new DB(Config::dbFile);
	session = g_SessionID;
	time(&start);
	Logger::get_instance()->info("[{}] Session started",session);
}
void Journal::Open() {
#ifdef ENABLE_DB
	db->Open(g_SessionID, start);
#endif
}
void Journal::Record(Event::Journal e) {
	Logger::get_instance()->info("[{}][{}] {}", e.time,Utils::toStr(e.id),Utils::toStr(e.type));
	MQ::Send(e);
#ifdef ENABLE_DB
	db->Insert(e);
#endif
}
void Journal::Close() {
	time_t end;
	time(&end);
#ifdef ENABLE_DB
	db->Close(end);
#endif
	g_SessionID = "";
	delete db;
}