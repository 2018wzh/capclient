#pragma once
#include <string>
namespace Config {
	extern std::string mqServer, loginServer, loginToken,mqUser,mqPass, dbFile, configFile;
}
void readConfig();