#include "config.h"
#include <json/json.h>
#include <fstream>
#include "logger.h"
namespace Config {
    std::string mqServer, loginServer, loginToken, mqUser, mqPass, dbFile;
    std::string configFile = "config.json";
}
void readConfig() {
    std::ifstream file(Config::configFile);
    if (!file.is_open()) {
        Json::Value root;
        root["mqServer"] = "";
        root["loginServer"] = "";
        root["loginToken"] = "";
        root["mqUser"] = "";
		root["mqPass"] = "";
        root["dbFile"] = "journal.db";
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
    }
    catch (std::exception& e) {
        Logger::get_instance()->error(e.what());
        throw std::runtime_error("Config file is corrupted");
    }
}