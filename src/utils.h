#pragma once
#include <string>
void vkMapInit();
std::string vkConvert(int vkCode);
void wmMapInit();
std::string wmConvert(int vkCode);
bool checkAdm();
bool elevateAdm();