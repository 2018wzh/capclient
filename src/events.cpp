#include "events.h"
baseEvent::baseEvent() {
	id = uuidxx::uuid::Generate();
}
mouseEvent::mouseEvent(WPARAM w, MSLLHOOKSTRUCT* ks) {
	id = uuidxx::uuid::Generate();
	time = ks->time;
	type = w;
	posX = ks->pt.x;
	posY = ks->pt.y;
	friendlyName = wmConvert(type);
}
kbdEvent::kbdEvent(KBDLLHOOKSTRUCT* ks) {
	id = uuidxx::uuid::Generate();
	time = ks->time;
	type = ks->vkCode;
	friendlyName = vkConvert(type);
}
screenEvent::screenEvent(DWORD t,std::string file) {
	id = uuidxx::uuid::Generate();
	time = t;
	content = file;
}