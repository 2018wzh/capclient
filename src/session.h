#pragma once
#include "uuidxx.h"
#include <rapidjson/document.h>
#include <fstream>
class Session {
private:
	uuidxx::uuid sId;
	std::ofstream outFile;
public:
	time_t sStartTime, sEndTime, sLength;
	rapidjson::Document sDoc;
	void Close();
	void Open();
};