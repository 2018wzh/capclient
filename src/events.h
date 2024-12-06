#pragma once
#include <string>
#include <windows.h>
#include "uuidxx.h"
#include <json/json.h>
#include "screenshot.h"
enum class EventType {
	Mouse,
	Keyboard,
	Screen
};
struct journalEvent {
	journalEvent(KBDLLHOOKSTRUCT* ks,bool isDown);
	journalEvent(MSLLHOOKSTRUCT* ks, WPARAM w,bool isDown);
	journalEvent(DWORD time);
	uuidxx::uuid id;
	std::string uid;
	EventType type;
	int time,value=0;
	std::string data="", friendly;
	std::string toJson();
};