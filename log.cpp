#include"log.h"
#include<windows.h>
#include<iostream>
#include<fstream>
//std::ofstream gLog("out.log", std::ios::out);
#define gLog std::cerr
void logger(mouseEvent e) {
	gLog << "[" << e.time << "] " << e.type << "->" << e.friendlyName << " pressed at (" << e.posX << "," << e.posY << ") " << std::endl;
}
void logger(kbdEvent e) {
	gLog << "[" << e.time << "] " << e.type << "->" << e.friendlyName << std::endl;
}
void logger(screenEvent e) {

}

void logger(int level, std::string msg) {
	gLog << msg << std::endl;
}