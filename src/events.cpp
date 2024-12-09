#include "events.h"
#include "utils.h"
#include <ctime>
#include "ui.h"

Event::Journal::Journal(MSLLHOOKSTRUCT *ks, WPARAM w,bool isDown){
	id = uuidxx::uuid::Generate();
	type = Event::Type::Mouse;
	time = ks->time;
	if(isDown)
		data["action"] = "down";
	else
		data["action"] = "up";
	data["value"] = int(w);
	data["friendly"] = Utils::wmConvert(w);
	data["posX"] = int(ks->pt.x);
	data["posY"] = int(ks->pt.y);
}
Event::Journal::Journal(KBDLLHOOKSTRUCT* ks,bool isDown) {
	id = uuidxx::uuid::Generate();
	type = Event::Type::Keyboard;
	time = ks->time;
	if (isDown)
		data["action"] = "down";
	else
		data["action"] = "up";
	data["value"] = int(ks->vkCode);
	data["friendly"] = Utils::vkConvert(ks->vkCode);
}
Event::Journal::Journal(DWORD tm) {
	id = uuidxx::uuid::Generate();
	Json::StreamWriterBuilder writer;
	type = Event::Type::Screen;
	time = tm;
	data["screenshot"] = Screenshot::Make();
	friendly = "Screenshot";
}