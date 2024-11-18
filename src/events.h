#pragma once
#include <string>
#include <windows.h>
#include "utils.h"
#include "uuidxx.h"
#include "screenshot.h"
struct baseEvent {
	uuidxx::uuid id;
	DWORD time, type;
	baseEvent();
	bool operator <(const baseEvent& b) {
		return time < b.time;
	}
	std::string friendlyName;
};
struct mouseEvent :baseEvent {
	int posX, posY;
	mouseEvent(WPARAM w, MSLLHOOKSTRUCT* ks);
};
struct kbdEvent :baseEvent {
	kbdEvent(KBDLLHOOKSTRUCT* ks);
};
struct screenEvent :baseEvent {
	std::string content;
	screenEvent(DWORD t,std::string file);
};