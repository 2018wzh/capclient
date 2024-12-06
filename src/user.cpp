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

User* currentUser = nullptr;
void userLogin() {
	currentUser=new User;
    try {
        curlpp::Cleanup cleanup;
        std::ostringstream responseStream;
        std::string url = Config::loginServer + "/auth/user/captcha";
        curlpp::Easy request;
        request.setOpt(new curlpp::options::Url(url));
        request.setOpt(new curlpp::options::WriteStream(&responseStream));
        request.perform();
		std::string response = responseStream.str();
		Json::Value responseJson=toJson(response);
		currentUser->id = responseJson["result"].asString();
    }
    catch (std::exception& e) {
		Logger::get_instance()->error(e.what());
		throw std::exception("Login failed");
    }
}
void userLogout() {
	if (!checkLogin())
		throw std::exception("Not logged in");
	currentUser = nullptr;
}
inline bool checkLogin() {
	if (currentUser == nullptr)
		return false;
	return true;
}
User getUser() {
	return *currentUser;
}