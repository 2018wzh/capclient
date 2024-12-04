#include "user.h"
#include "logger.h"
#include <iostream>
User* currentUser = nullptr;
void userLogin() {
	std::cout << "userid:";
	currentUser=new User;
	std::cin >> currentUser->id;
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