#include "hook.h"
#include "logger.h"
#include "events.h"
#include "journal.h"
#include "screenshot.h"
#include "config.h"
#include <iostream>
#include <unordered_map>
#include <cassert>
#include <windows.h>
#include <thread>
#include "ui.h"
#include "cmd.h"
#include <ctime>
using namespace Hook;
std::thread Hook::Thread;
std::atomic<bool> Hook::Running = false;
HHOOK keyHook = nullptr, mseHook = nullptr;
DWORD last = 0, lastkey = 0;
namespace flags {
    bool Shift = false;
    bool Ctrl = false;
    bool Alt = false;
}
Journal* hookJournal = nullptr;

void Hook::ThreadFunc() {
	installHook();
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (msg.message == WM_QUIT)
            cmd::exit();
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	unInstallHook();
}
static LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode < 0) {
        return CallNextHookEx(keyHook, nCode, wParam, lParam);
    }

    KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
    if (last == wParam && lastkey == ks->vkCode) {
        return CallNextHookEx(keyHook, nCode, wParam, lParam);
    }
    last = wParam;
    lastkey = ks->vkCode;
    bool isDown = false;
    auto tm = time(NULL);
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        if ((flags::Ctrl&&flags::Shift)||(flags::Ctrl&&(ks->vkCode!=VK_LSHIFT||ks->vkCode!=VK_RSHIFT))||flags::Alt) {
            hookJournal->Record(Event::Journal(ks->time)); // screenshot
        }
        if (ks->vkCode == VK_SHIFT||ks->vkCode==VK_LSHIFT || ks->vkCode == VK_RSHIFT) {
            flags::Shift = 1;
            Logger::get_instance()->debug("Shift down");
        }
        if (ks->vkCode == VK_CONTROL || ks->vkCode == VK_LCONTROL||ks->vkCode==VK_RCONTROL) {
            flags::Ctrl = 1;
            Logger::get_instance()->debug("Ctrl down");
        }
        if (ks->vkCode == VK_MENU||ks->vkCode==VK_LMENU||ks->vkCode==VK_RMENU) {
            flags::Alt = 1;
            Logger::get_instance()->debug("Alt down");
        }
        isDown = true;
        hookJournal->Record(Event::Journal(ks, isDown, tm)); // keyboard
    }
    else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
        if (ks->vkCode == VK_SHIFT || ks->vkCode == VK_LSHIFT || ks->vkCode == VK_RSHIFT) {
            flags::Shift = 0;
            Logger::get_instance()->debug("Shift up");
        }
        if (ks->vkCode == VK_CONTROL || ks->vkCode == VK_LCONTROL || ks->vkCode == VK_RCONTROL) {
            flags::Ctrl = 0;
            Logger::get_instance()->debug("Ctrl up");
        }
        if (ks->vkCode == VK_MENU || ks->vkCode == VK_LMENU || ks->vkCode == VK_RMENU) {
            flags::Alt = 0;
            Logger::get_instance()->debug("Alt up");
        }
        Logger::get_instance()->debug("Key up: vkCode={}, time={}", ks->vkCode, ks->time);
        isDown = false;
    }
    return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

static LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode < 0) {
        return CallNextHookEx(mseHook, nCode, wParam, lParam);
    }
    auto tm = time(NULL);
    bool isDown = false;
    MSLLHOOKSTRUCT* ks = (MSLLHOOKSTRUCT*)lParam;
    if (wParam != WM_MOUSEMOVE) {
        if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN || wParam == WM_MBUTTONDOWN)
            isDown = true;
        hookJournal->Record(Event::Journal(ks, wParam, isDown, tm)); // mouse
        hookJournal->Record(Event::Journal(tm)); // screenshot
        last = ks->time;
    }
    return CallNextHookEx(mseHook, nCode, wParam, lParam);
}
void Hook::installHook(){
    hookJournal = new Journal;
	hookJournal->Open();
    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, nullptr, 0);
    mseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, nullptr, 0);
    if (!keyHook || !mseHook) {
        if (!keyHook)
            throw std::runtime_error("KeyHook failed");
        if (!mseHook)
            throw std::runtime_error("MouseHook failed");
    }
}

void Hook::unInstallHook() {
    if (mseHook) {
        UnhookWindowsHookEx(mseHook);
        mseHook = nullptr;
    }
    if (keyHook) {
        UnhookWindowsHookEx(keyHook);
        keyHook = nullptr;
    }
    if (hookJournal) {
        hookJournal->Close();
        delete hookJournal;
        hookJournal = nullptr;
    }
}