#pragma once
#include<string>
#include<windows.h>
#include "utils.h"
struct baseEvent {
	DWORD time,type;
	std::string friendlyName;
};
struct mouseEvent:baseEvent {
	int posX, posY;
	mouseEvent(WPARAM w,MSLLHOOKSTRUCT* ks) {
		time = ks->time;
		type = w;
		posX = ks->pt.x;
		posY = ks->pt.y;
		friendlyName = wmConvert(type);
	}
};
struct kbdEvent :baseEvent {
	kbdEvent(KBDLLHOOKSTRUCT* ks) {
		time = ks->time;
		type = ks->vkCode;
		friendlyName = vkConvert(type);
	}
};
struct screenEvent {
	DWORD time;
	std::wstring fileName;
	screenEvent(DWORD t, std::wstring f) {
		time = t;
		fileName = f;
	}
};