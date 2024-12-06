#pragma once
#include <string>
#include "events.h"
#include "uuidxx.h"
#include <json/json.h>
void vkMapInit();
std::string vkConvert(int vkCode);
void wmMapInit();
std::string wmConvert(int vkCode);
bool checkAdm();
bool elevateAdm();
std::string base64Encode(const unsigned char* data, size_t len);
std::string toStr(uuidxx::uuid input);
std::string toStr(EventType e);
Json::Value toJson(std::string s);