#pragma once
#include <string>
struct User {
	std::string id,name;
};
void userLogin();
void userLogout();
User getUser();
bool checkLogin();