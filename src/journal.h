#pragma once
#include "events.h"
void journalOpen();
void journalRecord(mouseEvent e);
void journalRecord(kbdEvent e);
void journalRecord(screenEvent e);
void journalClose();