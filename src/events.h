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
		Screen
	};
	struct Journal {
		Journal(KBDLLHOOKSTRUCT* ks, bool isDown);
		Journal(MSLLHOOKSTRUCT* ks, WPARAM w, bool isDown);
		Journal(DWORD time);
		uuidxx::uuid id;
		std::string uid;
		Type type;
		int time, value = 0;
		Json::Value data;
		std::string friendly;
		std::string toJson();
	};
	struct MQcmd {};
}