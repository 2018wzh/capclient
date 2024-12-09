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
	if (token.empty())
		throw std::exception("Token is empty");
	id = "";
	name = "";
	this->token = token;
	isLoggedin = false;
}
void User::Login() {
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
	isLoggedin = 1;
}
void User::Logout() {
	if (!isLoggedin)
		throw std::exception("Not logged in");
}
