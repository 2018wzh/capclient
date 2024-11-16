#pragma once
#include <windows.h>
class kbdHook
{
public:
    void installHook();
    void unInstallHook();
};
class mouseHook
{
public:
    void installHook();
    void unInstallHook();
};