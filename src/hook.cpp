#include "hook.h"
#include "log.h"
#include "events.h"
#include "journal.h"
#include "screenshot.h"
#include <iostream>
static HHOOK keyHook = nullptr;
static kbdHook* khook;
static mouseHook* mhook;
static HHOOK mseHook = nullptr;

LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
    journalRecord(ks);
    return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

void kbdHook::installHook() {
    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, nullptr, 0);
    if (!keyHook)
        std::cerr << "KeyboardHook failed" << std::endl;
    else
        std::cerr << "KeyboardHook success" << std::endl;
    khook = this;

}
void kbdHook::unInstallHook() {
    UnhookWindowsHookEx(keyHook);
    keyHook = nullptr;
}

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    MSLLHOOKSTRUCT* ks = (MSLLHOOKSTRUCT*)lParam;
    if (wParam != WM_MOUSEMOVE) {
        journalRecord(mouseEvent(wParam, ks));
        journalRecord(screenEvent(ks->time,mkScreenshot()));
    }
    return CallNextHookEx(mseHook, nCode, wParam, lParam);
}

void mouseHook::installHook() {
    mseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, nullptr, 0);
    if (!mseHook)
        std::cerr << "MouseHook failed" << std::endl;
    else
        std::cerr << "MouseHook success" << std::endl;
    mhook = this;
}
void mouseHook::unInstallHook() {
    UnhookWindowsHookEx(mseHook);
    mseHook = nullptr;
}