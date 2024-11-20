#include "utils.h"
#include<windows.h>
#include<tchar.h>
#include <unordered_map>
#include "logger.h"
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
    vkCodeToName[VK_CAPITAL] = "Caps";
    vkCodeToName[VK_ESCAPE] = "Escape";
    vkCodeToName[VK_SPACE] = "Space";
    vkCodeToName[VK_PRIOR] = "PgUp";
    vkCodeToName[VK_NEXT] = "PgDown";
    vkCodeToName[VK_END] = "End";
    vkCodeToName[VK_HOME] = "Home";
    vkCodeToName[VK_LEFT] = "LeftArrow";
    vkCodeToName[VK_UP] = "UpArrow";
    vkCodeToName[VK_RIGHT] = "RightArrow";
    vkCodeToName[VK_DOWN] = "DownArrow";
    vkCodeToName[VK_INSERT] = "Insert";
    vkCodeToName[VK_DELETE] = "Delete";
    vkCodeToName[VK_LWIN] = "LWindows";
    vkCodeToName[VK_RWIN] = "RWindows";
    vkCodeToName[VK_APPS] = "Application";
    vkCodeToName[VK_NUMPAD0] = "Num0";
    vkCodeToName[VK_NUMPAD1] = "Num1";
    vkCodeToName[VK_NUMPAD2] = "Num2";
    vkCodeToName[VK_NUMPAD3] = "Num3";
    vkCodeToName[VK_NUMPAD4] = "Num4";
    vkCodeToName[VK_NUMPAD5] = "Num5";
    vkCodeToName[VK_NUMPAD6] = "Num6";
    vkCodeToName[VK_NUMPAD7] = "Num7";
    vkCodeToName[VK_NUMPAD8] = "Num8";
    vkCodeToName[VK_NUMPAD9] = "Num9";
    vkCodeToName[VK_MULTIPLY] = "Num*";
    vkCodeToName[VK_ADD] = "Num+";
    vkCodeToName[VK_SEPARATOR] = "Separator";
    vkCodeToName[VK_SUBTRACT] = "Num-";
    vkCodeToName[VK_DECIMAL] = "Num.";
    vkCodeToName[VK_DIVIDE] = "Num/";
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
    vkCodeToName[VK_NUMLOCK] = "NumLock";
    vkCodeToName[VK_SCROLL] = "ScrollLock";
    vkCodeToName[VK_LSHIFT] = "LShift";
    vkCodeToName[VK_RSHIFT] = "RShift";
    vkCodeToName[VK_LCONTROL] = "LCtrl";
    vkCodeToName[VK_RCONTROL] = "RCtrl";
    vkCodeToName[VK_LMENU] = "LAlt";
    vkCodeToName[VK_RMENU] = "RAlt";
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
    wmCodeToName[WM_LBUTTONDOWN] = "LBDown";
    wmCodeToName[WM_LBUTTONUP] = "LBUp";
    wmCodeToName[WM_RBUTTONDOWN] = "RBDown";
    wmCodeToName[WM_RBUTTONUP] = "RBUp";
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
    Logger::get_instance()->warn("Elevating");
    TCHAR szPath[MAX_PATH];
    if (GetModuleFileName(NULL, szPath, MAX_PATH)) {
        SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
        sei.lpVerb = _T("runas");
        sei.lpFile = szPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;
        return ShellExecuteEx(&sei);
    }
    throw "Elevate failed";
    return 1;
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
std::string toStr(uuidxx::uuid u) {
    std::string o = u.ToString();
    return o.substr(1, o.length() - 2);
}
std::string toStr(EventType e){
    switch (e) {
    case EventType::Mouse:
        return "mouse";
    case EventType::Keyboard:
        return "keyboard";
    case EventType::Screen:
        return "screenshot";
    default:
        return "undefined";
    }
}