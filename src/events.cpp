#include "events.h"
#include "utils.h"
#include <ctime>
#include "ui.h"
#include <json/json.h>
journalEvent::journalEvent(MSLLHOOKSTRUCT* ks, WPARAM w,bool isDown) {
	id = uuidxx::uuid::Generate();
	Json::StreamWriterBuilder writer;
	writer["indentation"] = "";
	type = EventType::Mouse;
	time = ks->time;
	Json::Value doc;
	
	if (isDown)
		//getUIElement(ks->pt);
	
	if(isDown)
		doc["action"] = "down";
	else
		doc["action"] = "up";
	doc["value"] = int(w);
	doc["friendly"] = wmConvert(w);
	doc["posX"] = int(ks->pt.x);
	doc["posY"] = int(ks->pt.y);
	data = Json::writeString(writer, doc);
}
journalEvent::journalEvent(KBDLLHOOKSTRUCT* ks,bool isDown) {
	id = uuidxx::uuid::Generate();
	Json::StreamWriterBuilder writer;
	writer["indentation"] = "";
	type = EventType::Keyboard;
	time = ks->time;
	Json::Value doc;
	if (isDown)
		doc["action"] = "down";
	else
		doc["action"] = "up";
	doc["value"] = int(ks->vkCode);
	doc["friendly"] = vkConvert(ks->vkCode);
	data = Json::writeString(writer, doc);
}
journalEvent::journalEvent(DWORD tm) {
	id = uuidxx::uuid::Generate();
	Json::StreamWriterBuilder writer;
	type = EventType::Screen;
	time = tm;
	data = mkScreenshot();
	friendly = "Screenshot";
}
