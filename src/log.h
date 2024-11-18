#pragma once
#include "events.h"
#include <string>
#include<fstream>
enum class LOGLEVEL;
void logger(LOGLEVEL, std::string msg);