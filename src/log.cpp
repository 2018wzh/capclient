#include"log.h"
#include<windows.h>
#include<iostream>
#include<fstream>
//std::ofstream gLog("out.log", std::ios::out);
#define gLog std::cerr
enum class LOGLEVEL {
	debug,
	info,
	warning,
	error,
	critical
};

void logger(LOGLEVEL, std::string msg) {
	gLog << msg << std::endl;
}