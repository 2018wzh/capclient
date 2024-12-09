#include "events.h"
#include "utils.h"
#include <ctime>
#include "ui.h"

Event::Journal::Journal(MSLLHOOKSTRUCT *ks, WPARAM w,bool isDown){
	id = uuidxx::uuid::Generate();
	Json::StreamWriterBuilder writer;
	writer["indentation"] = "";
	type = Event::Type::Mouse;
	time = ks->time;
	Json::Value doc;
	if(isDown)
		doc["action"] = "down";
	else
		doc["action"] = "up";
	doc["value"] = int(w);
	doc["friendly"] = Utils::wmConvert(w);
	doc["posX"] = int(ks->pt.x);
	doc["posY"] = int(ks->pt.y);
	data = Json::writeString(writer, doc);
}
Event::Journal::Journal(KBDLLHOOKSTRUCT* ks,bool isDown) {
	id = uuidxx::uuid::Generate();
	Json::StreamWriterBuilder writer;
	writer["indentation"] = "";
	type = Event::Type::Keyboard;
	time = ks->time;
	Json::Value doc;
	if (isDown)
		doc["action"] = "down";
	else
		doc["action"] = "up";
	doc["value"] = int(ks->vkCode);
	doc["friendly"] = Utils::vkConvert(ks->vkCode);
	data = Json::writeString(writer, doc);
}
Event::Journal::Journal(DWORD tm) {
	id = uuidxx::uuid::Generate();
	Json::StreamWriterBuilder writer;
	type = Event::Type::Screen;
	time = tm;
	data = Screenshot::Make();
	friendly = "Screenshot";
}
std::string Event::Journal::toJson() {
	Json::StreamWriterBuilder writer;
	writer["indentation"] = "";
	Json::Value doc;
	doc["id"] = Utils::toStr(id);
	doc["uid"] = uid;
	doc["type"] = Utils::toStr(type);
	doc["time"] = time;
	doc["data"] = data;
	doc["friendly"] = friendly;
	return Json::writeString(writer, doc);
}