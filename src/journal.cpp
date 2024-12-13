#include "journal.h"
#include "utils.h"
#include "logger.h"
#include "mq.h"
#include "user.h"
#include "config.h"
uuidxx::uuid g_SessionID;
Journal::Journal() {
	db = new DB(Config::dbFile);
	sessionID = uuidxx::uuid::Generate();
	time(&start);
	Logger::get_instance()->info("[{}] Session started",Utils::toStr(sessionID));
}
void Journal::Open() {
	g_SessionID = this->sessionID;
	db->Open(Utils::toStr(sessionID), start);
	//MQ::Send(Event::Journal(start, "start"));
}
void Journal::Record(Event::Journal e) {
	Logger::get_instance()->info("[{}][{}] {}", e.time,Utils::toStr(e.id),Utils::toStr(e.type));
	//currentMQ->Send(e);
	MQ::Send(e);
	db->Insert(e);
}
void Journal::Close() {
	time_t end;
	time(&end);
	//MQ::Send(Event::Journal(end, "end"));
	db->Close(end);
	g_SessionID = NULL;
	delete db;
}