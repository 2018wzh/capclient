#pragma once
#include <string>
#include "events.h"
#include "uuidxx.h"
#include <json/json.h>
namespace Utils {
	void vkMapInit();
	std::string vkConvert(int vkCode);
	void wmMapInit();
	std::string wmConvert(int vkCode);
	bool checkAdm();
	bool elevateAdm();
	std::string base64Encode(const unsigned char* data, size_t len);
	std::string toStr(uuidxx::uuid input);
	std::string toStr(Event::Type e);
	std::string toStr(Json::Value j);
	std::string toStr(Event::Journal j);
	Json::Value toJson(std::string s);
	std::string toStr(std::wstring wstr);
};