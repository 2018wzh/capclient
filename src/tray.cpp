#include "tray.h"
#include <stdexcept>
#include "cmd.h"
#include "config.h" // 包含配置头文件
#include "mq.h"
#include "hook.h"
#include "user.h"
#include <tchar.h> // 添加头文件
#include <string>

Tray::~Tray()
{
    Shell_NotifyIcon(NIM_DELETE, &nid_);
}

void Tray::Init()
{
    // 注册窗口类
    WNDCLASS wndClass = {};
    wndClass.lpfnWndProc = Tray::WndProc;
    wndClass.hInstance = hInstance_;
    wndClass.lpszClassName = TEXT("TrayClass");
    if (!RegisterClass(&wndClass))
    {
        throw std::runtime_error("Register Class failed");
    }
    // 创建消息窗口
    hwnd_ = CreateWindow(TEXT("TrayClass"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance_, NULL);
    if (!hwnd_)
    {
        throw std::runtime_error("Create Window failed");
    }

    // 设置窗口的用户数据
    SetWindowLongPtr(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    // 初始化托盘图标数据
    nid_.cbSize = sizeof(NOTIFYICONDATA);
    nid_.hWnd = hwnd_;
    nid_.uID = 1;
    nid_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid_.uCallbackMessage = WM_USER + 1;
    nid_.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    lstrcpy(nid_.szTip, TEXT("SXFX Capture Client"));

    // 添加托盘图标
    if (!Shell_NotifyIcon(NIM_ADD, &nid_))
        throw std::runtime_error("Add NotifyIcon failed");
}

void Tray::Show()
{
    nid_.uFlags = NIF_STATE;
    nid_.dwStateMask = NIS_HIDDEN;
    nid_.dwState = 0;
    Shell_NotifyIcon(NIM_MODIFY, &nid_);
}

void Tray::Hide()
{
    nid_.uFlags = NIF_STATE;
    nid_.dwStateMask = NIS_HIDDEN;
    nid_.dwState = NIS_HIDDEN;
    Shell_NotifyIcon(NIM_MODIFY, &nid_);
}

void Tray::ShowContextMenu(HWND hwnd, POINT pt)
{
    HMENU hMenu = CreatePopupMenu();
    if (!hMenu)
        throw std::runtime_error("Create PopupMenu failed");

    // 获取运行状态
    bool mq_running = MQ::Running.load();
    bool hook_running = Hook::Running.load();
    bool user_status = currentUser->Check();

    // 使用wstring构造状态文本
    std::wstring mqStatusText = L"MQ" + std::wstring(mq_running ? L"运行中" : L"已停止");
    std::wstring hookStatusText = L"Hook" + std::wstring(hook_running ? L"运行中" : L"已停止");
    std::wstring userStatusText;
    if (user_status)
        userStatusText = currentUser->name;
    else
        userStatusText = L"未登录";
    // 添加状态菜单项（灰色不可点击）
    InsertMenu(hMenu, -1, MF_BYPOSITION | MF_GRAYED, 0, userStatusText.c_str());
    InsertMenu(hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    InsertMenu(hMenu, -1, MF_BYPOSITION | MF_GRAYED, 0, mqStatusText.c_str());
    InsertMenu(hMenu, -1, MF_BYPOSITION | MF_GRAYED, 0, hookStatusText.c_str());

    // 添加分隔符
    InsertMenu(hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);

    // 添加其他菜单项
    InsertMenu(hMenu, -1, MF_BYPOSITION, IDM_CONFIG, L"设置");
    if (user_status)
        InsertMenu(hMenu, -1, MF_BYPOSITION, IDM_LOGOUT, L"注销");
    else
        InsertMenu(hMenu, -1, MF_BYPOSITION, IDM_LOGIN, L"登录");
    InsertMenu(hMenu, -1, MF_BYPOSITION, IDM_EXIT, L"退出");

    // 设置前景窗口
    SetForegroundWindow(hwnd);

    // 显示菜单
    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);

    DestroyMenu(hMenu);
}

LRESULT CALLBACK Tray::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_USER + 1 && lParam == WM_RBUTTONUP)
    {
        POINT pt;
        GetCursorPos(&pt);
        Tray *tray = reinterpret_cast<Tray *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (tray)
        {
            tray->ShowContextMenu(hwnd, pt);
        }
        return 0;
    }
    else if (message == WM_COMMAND)
    {
        switch (LOWORD(wParam))
        {
        case IDM_LOGIN:
            cmd::login();
            break;
        case IDM_LOGOUT:
            cmd::logout();
            break;
        case IDM_CONFIG:
            break;
        case IDM_EXIT:
            // 处理“退出”菜单项
            cmd::exit();
            PostQuitMessage(0);
            break;
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
