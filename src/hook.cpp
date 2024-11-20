#include "hook.h"
#include "logger.h"
#include "events.h"
#include "journal.h"
#include "screenshot.h"
#include <iostream>
HHOOK keyHook, mseHook;
DWORD last=0;
Journal hookJournal("journal.db");
LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
       hookJournal.Record(journalEvent(ks));         //keyboard
    if (ks->vkCode == VK_ESCAPE) {
        Logger::get_instance()->info("ESC pressed, exiting");
        PostQuitMessage(0);
    }
    return CallNextHookEx(keyHook, nCode, wParam, lParam);
}
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    MSLLHOOKSTRUCT* ks = (MSLLHOOKSTRUCT*)lParam;
    if (wParam != WM_MOUSEMOVE && ks->time-last>=1000) {
        hookJournal.Record(journalEvent(ks,wParam));          //mouse
        hookJournal.Record(journalEvent(ks->time));           //screenshot
        last = ks->time;
    }
    return CallNextHookEx(mseHook, nCode, wParam, lParam);
}

bool Hook::installHook() {
    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, nullptr, 0);
    mseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, nullptr, 0);
    if (!keyHook || !mseHook) {
        if(!keyHook)
            Logger::get_instance()->critical("KeyboardHook failed");
        if(!mseHook)
            Logger::get_instance()->critical("MouseHook failed");
        return 0;
    }
    Logger::get_instance()->info("Hook success");
    return 1;
}
void Hook::unInstallHook() {
    UnhookWindowsHookEx(mseHook);
    mseHook = nullptr;
    UnhookWindowsHookEx(keyHook);
    keyHook = nullptr;
    hookJournal.Close();
}