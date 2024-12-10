#pragma once
#include <string>
namespace Config {
	extern std::string mqServer, loginServer, loginToken,mqUser,mqPass, dbFile, configFile, mqVHost, mqName;
	extern int jpegQuality;
	void Read();
	void Save();
}