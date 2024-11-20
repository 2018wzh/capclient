#include "events.h"
#include "utils.h"
#include <ctime>
#include <json/json.h>
Json::StreamWriterBuilder writer;
journalEvent::journalEvent(MSLLHOOKSTRUCT* ks, WPARAM w) {
	writer["indentation"] = "";
	id = uuidxx::uuid::Generate();
	type = EventType::Mouse;
	time = ks->time;
	Json::Value doc;
	doc["value"] = int(w);
	doc["friendly"] = wmConvert(w);
	doc["posX"] = int(ks->pt.x);
	doc["posY"] = int(ks->pt.y);
	data = Json::writeString(writer, doc);
}
journalEvent::journalEvent(KBDLLHOOKSTRUCT* ks) {
	writer["indentation"] = "";
	id = uuidxx::uuid::Generate();
	type = EventType::Keyboard;
	time = ks->time;
	Json::Value doc;
	doc["value"] = int(ks->vkCode);
	doc["friendly"] = vkConvert(ks->vkCode);
	data = Json::writeString(writer, doc);
}
journalEvent::journalEvent(DWORD tm) {
	id = uuidxx::uuid::Generate();
	type = EventType::Screen;
	time = tm;
	data = mkScreenshot();
	friendly = "Screenshot";
}
