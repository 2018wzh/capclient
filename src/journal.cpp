#include "journal.h"
#include <iostream>
#define gLog std::cerr
void journalOpen();
void journalRecord(mouseEvent e) {
	gLog << "[" << e.time << "] " << e.type << "->" << e.friendlyName << " pressed at (" << e.posX << "," << e.posY << ") " << std::endl;
}
void journalRecord(kbdEvent e) {
	gLog << "[" << e.time << "] " << e.type << "->" << e.friendlyName << std::endl;
}
void journalRecord(screenEvent e) {
	gLog << "[" << e.time << "] " << e.type << "->" << e.friendlyName<<std::endl;
}
void journalClose();