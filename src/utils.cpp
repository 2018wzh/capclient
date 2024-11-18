#include "utils.h"
#include<windows.h>
#include<tchar.h>
#include <unordered_map>

std::unordered_map<int, std::string> vkCodeToName;
bool vkInit = 0;
void vkMapInit() {
    vkCodeToName[VK_BACK] = "Backspace";
    vkCodeToName[VK_TAB] = "Tab";
    vkCodeToName[VK_RETURN] = "Enter";
    vkCodeToName[VK_SHIFT] = "Shift";
    vkCodeToName[VK_CONTROL] = "Ctrl";
    vkCodeToName[VK_MENU] = "Alt";
    vkCodeToName[VK_PAUSE] = "Pause";
    vkCodeToName[VK_CAPITAL] = "Caps Lock";
    vkCodeToName[VK_ESCAPE] = "Escape";
    vkCodeToName[VK_SPACE] = "Space";
    vkCodeToName[VK_PRIOR] = "Page Up";
    vkCodeToName[VK_NEXT] = "Page Down";
    vkCodeToName[VK_END] = "End";
    vkCodeToName[VK_HOME] = "Home";
    vkCodeToName[VK_LEFT] = "Left Arrow";
    vkCodeToName[VK_UP] = "Up Arrow";
    vkCodeToName[VK_RIGHT] = "Right Arrow";
    vkCodeToName[VK_DOWN] = "Down Arrow";
    vkCodeToName[VK_INSERT] = "Insert";
    vkCodeToName[VK_DELETE] = "Delete";
    vkCodeToName[VK_LWIN] = "Left Windows";
    vkCodeToName[VK_RWIN] = "Right Windows";
    vkCodeToName[VK_APPS] = "Application";
    vkCodeToName[VK_NUMPAD0] = "Numpad 0";
    vkCodeToName[VK_NUMPAD1] = "Numpad 1";
    vkCodeToName[VK_NUMPAD2] = "Numpad 2";
    vkCodeToName[VK_NUMPAD3] = "Numpad 3";
    vkCodeToName[VK_NUMPAD4] = "Numpad 4";
    vkCodeToName[VK_NUMPAD5] = "Numpad 5";
    vkCodeToName[VK_NUMPAD6] = "Numpad 6";
    vkCodeToName[VK_NUMPAD7] = "Numpad 7";
    vkCodeToName[VK_NUMPAD8] = "Numpad 8";
    vkCodeToName[VK_NUMPAD9] = "Numpad 9";
    vkCodeToName[VK_MULTIPLY] = "Numpad *";
    vkCodeToName[VK_ADD] = "Numpad +";
    vkCodeToName[VK_SEPARATOR] = "Separator";
    vkCodeToName[VK_SUBTRACT] = "Numpad -";
    vkCodeToName[VK_DECIMAL] = "Numpad .";
    vkCodeToName[VK_DIVIDE] = "Numpad /";
    vkCodeToName[VK_F1] = "F1";
    vkCodeToName[VK_F2] = "F2";
    vkCodeToName[VK_F3] = "F3";
    vkCodeToName[VK_F4] = "F4";
    vkCodeToName[VK_F5] = "F5";
    vkCodeToName[VK_F6] = "F6";
    vkCodeToName[VK_F7] = "F7";
    vkCodeToName[VK_F8] = "F8";
    vkCodeToName[VK_F9] = "F9";
    vkCodeToName[VK_F10] = "F10";
    vkCodeToName[VK_F11] = "F11";
    vkCodeToName[VK_F12] = "F12";
    vkCodeToName[VK_NUMLOCK] = "Num Lock";
    vkCodeToName[VK_SCROLL] = "Scroll Lock";
    vkCodeToName[VK_LSHIFT] = "Left Shift";
    vkCodeToName[VK_RSHIFT] = "Right Shift";
    vkCodeToName[VK_LCONTROL] = "Left Ctrl";
    vkCodeToName[VK_RCONTROL] = "Right Ctrl";
    vkCodeToName[VK_LMENU] = "Left Alt";
    vkCodeToName[VK_RMENU] = "Right Alt";
    vkInit = 1;
}
std::string vkConvert(int vkCode) {
    if (!vkInit)
        vkMapInit();
    if (vkCodeToName.find(vkCode) != vkCodeToName.end()) {
        return vkCodeToName[vkCode];
    }

    if ((vkCode >= 'A' && vkCode <= 'Z') || (vkCode >= '0' && vkCode <= '9')) {
        return std::string(1, static_cast<char>(vkCode));
    }

    return "Unknown";
}

std::unordered_map<int, std::string> wmCodeToName;
bool wmInit = 0;
void wmMapInit() {
    wmCodeToName[WM_MOUSEMOVE] = "Move";
    wmCodeToName[WM_LBUTTONDOWN] = "LB Down";
    wmCodeToName[WM_LBUTTONUP] = "LB Up";
    wmCodeToName[WM_RBUTTONDOWN] = "RB Down";
    wmCodeToName[WM_RBUTTONUP] = "RB Up";
    wmInit = 1;
}
std::string wmConvert(int vkCode) {
    if (!wmInit)
        wmMapInit();
    if (wmCodeToName.find(vkCode) != wmCodeToName.end()) {
        return wmCodeToName[vkCode];
    }

    return "Unknown";
}
bool checkAdm() {
    BOOL isAdmin = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD dwSize;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            isAdmin = elevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }
    return isAdmin;
}
bool elevateAdm() {
    TCHAR szPath[MAX_PATH];
    if (GetModuleFileName(NULL, szPath, MAX_PATH)) {
        SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
        sei.lpVerb = _T("runas");
        sei.lpFile = szPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;
        return ShellExecuteEx(&sei);
    }
}

std::string base64Encode(const unsigned char* data, size_t len) {
    static const char base64_chars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string encoded;
    int i = 0;
    int val = 0;
    int valb = -6;

    for (size_t j = 0; j < len; ++j) {
        val = (val << 8) + data[j];
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6) {
        encoded.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }

    while (encoded.size() % 4) {
        encoded.push_back('=');
    }

    return encoded;
}