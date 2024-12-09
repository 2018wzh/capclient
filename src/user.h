#pragma once
#include <string>
class User {
private:
	std::string token = "";
	bool isLoggedin = 0;
public:
	std::string id = "", name = "";
	User(std::string token);
	void Login();
	void Logout();
	bool Check() { return isLoggedin; }
};
extern User* currentUser;