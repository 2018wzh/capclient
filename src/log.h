#pragma once
#include "events.h"
#include <string>
#include<fstream>
namespace LOGLEVEL {
	const int DBG = 0;
	const int INFO = 1;
	const int WARN = 2;
	const int ERR = 3;
	const int CRIT = 4;
}
void logger(mouseEvent e);
void logger(kbdEvent e);
void logger(screenEvent e);
void logger(int level, std::string msg);