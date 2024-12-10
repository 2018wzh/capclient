#include "config.h"
#include <json/json.h>
#include <fstream>
#include "logger.h"
namespace Config {
    std::string mqServer, loginServer, loginToken, mqUser, mqPass, dbFile, mqVHost, mqName;
    int jpegQuality;
    std::string configFile = "config.json";
}
void Config::Read() {
    std::ifstream file(Config::configFile);
    if (!file.is_open()) {
        Json::Value root;
        root["mqServer"] = "";
        root["loginServer"] = "";
        root["loginToken"] = "";
        root["mqUser"] = "";
		root["mqPass"] = "";
        root["dbFile"] = "journal.db";
        root["jpegQuality"] = 50;
		root["mqVHost"] = "";
        root["mqName"] = "";
        std::ofstream outFile(Config::configFile);
        if (!outFile.is_open()) {
            Logger::get_instance()->error("Unable to create default config file");
            throw std::runtime_error("Unable to create default config file");
        }
        outFile << root;
        outFile.close();


		throw std::runtime_error("Config file not found, default created");
    }
    Json::Value root;
    file >> root;
    try {
        Config::mqServer = root["mqServer"].asString();
        Config::loginServer = root["loginServer"].asString();
        Config::loginToken = root["loginToken"].asString();
        Config::mqPass = root["mqPass"].asString();
		Config::mqUser = root["mqUser"].asString();
		Config::dbFile = root["dbFile"].asString();
		Config::jpegQuality = root["jpegQuality"].asInt();
		Config::mqVHost = root["mqVHost"].asString();
        Config::mqName = root["mqName"].asString();
    }
    catch (std::exception& e) {
        Logger::get_instance()->error(e.what());
        throw std::runtime_error("Config file is corrupted");
    }   
}
void Config::Save() {
	Json::Value root;
	root["mqServer"] = Config::mqServer;
	root["loginServer"] = Config::loginServer;
	root["loginToken"] = Config::loginToken;
	root["mqUser"] = Config::mqUser;
	root["mqPass"] = Config::mqPass;
	root["dbFile"] = Config::dbFile;
	root["jpegQuality"] = Config::jpegQuality;
	root["mqVHost"] = Config::mqVHost;
    root["mqName"] = Config::mqName;
	std::ofstream outFile(Config::configFile);
	if (!outFile.is_open()) {
		Logger::get_instance()->error("Unable to save config file");
		throw std::runtime_error("Unable to save config file");
	}
	outFile << root;
	outFile.close();
}