#pragma once
#include <string>
class User {
private:
	std::string token = "";
	bool status = 0;
public:
	std::string id;
	std::wstring name;
	User() : name(L"Default"), id("null"){};
	User(std::string token);
	void Login();
	void Logout();
	inline bool Check() { return status; }
};
extern User* currentUser;