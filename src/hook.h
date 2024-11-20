#pragma once
#include <windows.h>
#include "journal.h"
class Hook
{
public:
    bool installHook();
    void unInstallHook();
};
