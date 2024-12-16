#include "user.h"
#include "logger.h"
#include "config.h"
#include "utils.h"
#include <json/json.h>
#include <iostream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <sstream>
User* currentUser;

User::User(std::string token) {
	this->id = "default";
	this->name = L"NULL";
	this->token = token;
	status = 0;
}
void User::Login() {
	/*
    try {
        curlpp::Cleanup cleanup;
        std::ostringstream responseStream;
        std::string url = Config::loginServer + "/auth/user/captcha";
        curlpp::Easy request;
        request.setOpt(new curlpp::options::Url(url));
        request.setOpt(new curlpp::options::WriteStream(&responseStream));
        request.perform();
		std::string response = responseStream.str();
		Json::Value responseJson=Utils::toJson(response);
		id = responseJson["result"].asString();
    }
    catch (std::exception& e) {
		Logger::get_instance()->error(e.what());
		throw std::exception("Login failed");
    }
	*/
	id = Config::userName;
	status = 1;
}
void User::Logout() {
	if (!status)
		throw std::exception("Not logged in");
	status = 0;
}
