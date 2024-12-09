#pragma once
#include <string>
namespace Config {
	extern std::string mqServer, loginServer, loginToken,mqUser,mqPass, dbFile, configFile;
	extern int jpegQuality;
	void Read();
	void Save();
}