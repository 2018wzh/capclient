#pragma once
#include <windows.h>
#include <string>

#define IDM_EXIT 1001
#define IDM_LOGIN 1002
#define IDM_LOGOUT 1003
#define IDM_CONFIG 1004

class Tray {
public:
    Tray(HINSTANCE hInstance) : hInstance_(hInstance), hwnd_(NULL) {};
    ~Tray();
    void Init();
    void Show();
    void Hide();

private:
    NOTIFYICONDATA nid_;
    HWND hwnd_;
    HINSTANCE hInstance_;
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    void ShowContextMenu(HWND hwnd, POINT pt);
};
