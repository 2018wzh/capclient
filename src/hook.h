#pragma once
#include <windows.h>
#include "journal.h"
const int interval = 500;
class Hook
{
public:
    bool installHook();
    void unInstallHook();
};
