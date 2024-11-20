#include "session.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include <ctime>
#include "utils.h"
void Session::Open() {
	time_t curtime;
	time(&curtime);
	sId = uuidxx::uuid::Generate();
	sStartTime = curtime;
	std::string sIdstr = uuid2str(sId);
	outFile.open(sIdstr + ".json", std::ios::out);
	rapidjson::Value sessionData;
	rapidjson::Document::AllocatorType& alloc = sDoc.GetAllocator();
	sessionData.AddMember("id", sIdstr, alloc);
	sessionData.AddMember("startTime", sStartTime, alloc);
	sDoc.AddMember("session", sessionData, alloc);
}
void Session::Close() {
	time_t curtime;
	time(&curtime);
	rapidjson::Document::AllocatorType& alloc = sDoc.GetAllocator();
	rapidjson::Value& sessionData= sDoc["session"];
	sEndTime = curtime;
	sessionData.AddMember("endTime", sEndTime, alloc);
	sessionData.AddMember("duration", sEndTime - sStartTime, alloc);
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	sDoc.Accept(writer);
	outFile << sDoc.GetString();
}
