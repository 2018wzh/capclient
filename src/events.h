#pragma once
#include <string>
#include <windows.h>
#include "uuidxx.h"
#include <json/json.h>
#include "screenshot.h"

namespace Event {
	enum class Type {
		Mouse,
		Keyboard,
		Screen,
		Control,
	};
	struct Journal {
		Journal(KBDLLHOOKSTRUCT* ks, bool isDown,time_t time);
		Journal(MSLLHOOKSTRUCT* ks, WPARAM w, bool isDown,time_t time);
		Journal(time_t time);
		Journal(std::string ctrltype);
		uuidxx::uuid id;
		std::string uid;
		Type type;
		time_t time;
		int value = 0;
		Json::Value data;
	};
	struct MQcmd {};
}