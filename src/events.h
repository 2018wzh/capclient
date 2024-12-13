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
	enum class Control {
		START,
		STOP
	};
	struct Journal {
		Journal(time_t time,KBDLLHOOKSTRUCT* ks, bool isDown);
		Journal(time_t time, MSLLHOOKSTRUCT* ks, WPARAM w, bool isDown);
		Journal(time_t time);
		Journal(time_t time, std::string ctrltype);
		uuidxx::uuid id,session;
		std::string uid="1";
		Type type;
		time_t time;
		int value = 0;
		Json::Value data;
	};
}