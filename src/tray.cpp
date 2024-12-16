#include "tray.h"
#include <stdexcept>
#include "cmd.h"
#include "config.h" // ��������ͷ�ļ�
#include "mq.h"
#include "hook.h"
#include "user.h"
#include <tchar.h> // ���ͷ�ļ�
#include <string>

Tray::~Tray()
{
    Shell_NotifyIcon(NIM_DELETE, &nid_);
}

void Tray::Init()
{
    // ע�ᴰ����
    WNDCLASS wndClass = {};
    wndClass.lpfnWndProc = Tray::WndProc;
    wndClass.hInstance = hInstance_;
    wndClass.lpszClassName = TEXT("TrayClass");
    if (!RegisterClass(&wndClass))
    {
        throw std::runtime_error("Register Class failed");
    }
    // ������Ϣ����
    hwnd_ = CreateWindow(TEXT("TrayClass"), NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance_, NULL);
    if (!hwnd_)
    {
        throw std::runtime_error("Create Window failed");
    }

    // ���ô��ڵ��û�����
    SetWindowLongPtr(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    // ��ʼ������ͼ������
    nid_.cbSize = sizeof(NOTIFYICONDATA);
    nid_.hWnd = hwnd_;
    nid_.uID = 1;
    nid_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid_.uCallbackMessage = WM_USER + 1;
    nid_.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    lstrcpy(nid_.szTip, TEXT("SXFX Capture Client"));

    // �������ͼ��
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

    // ��ȡ����״̬
    bool mq_running = MQ::Running.load();
    bool hook_running = Hook::Running.load();
    bool user_status = currentUser->Check();

    // ʹ��wstring����״̬�ı�
    std::wstring mqStatusText = L"MQ" + std::wstring(mq_running ? L"������" : L"��ֹͣ");
    std::wstring hookStatusText = L"Hook" + std::wstring(hook_running ? L"������" : L"��ֹͣ");
    std::wstring userStatusText;
    if (user_status)
        userStatusText = currentUser->name;
    else
        userStatusText = L"δ��¼";
    // ���״̬�˵����ɫ���ɵ����
    InsertMenu(hMenu, -1, MF_BYPOSITION | MF_GRAYED, 0, userStatusText.c_str());
    InsertMenu(hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    InsertMenu(hMenu, -1, MF_BYPOSITION | MF_GRAYED, 0, mqStatusText.c_str());
    InsertMenu(hMenu, -1, MF_BYPOSITION | MF_GRAYED, 0, hookStatusText.c_str());

    // ��ӷָ���
    InsertMenu(hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);

    // ��������˵���
    InsertMenu(hMenu, -1, MF_BYPOSITION, IDM_CONFIG, L"����");
    if (user_status)
        InsertMenu(hMenu, -1, MF_BYPOSITION, IDM_LOGOUT, L"ע��");
    else
        InsertMenu(hMenu, -1, MF_BYPOSITION, IDM_LOGIN, L"��¼");
    InsertMenu(hMenu, -1, MF_BYPOSITION, IDM_EXIT, L"�˳�");

    // ����ǰ������
    SetForegroundWindow(hwnd);

    // ��ʾ�˵�
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
            // �����˳����˵���
            cmd::exit();
            PostQuitMessage(0);
            break;
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
