#include "notifyicon.h"
#include "res/resource.h"
#include <tchar.h>
#include <stdio.h>
#include <commctrl.h>


NotifyIcon::NotifyIcon(HINSTANCE hInstance, HWND hWnd)
{
    ZeroMemory(&this->notifyIconData, sizeof(NOTIFYICONDATA));

    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    
    LoadIconMetric(hInstance, MAKEINTRESOURCE(IDI_ICON), LIM_SMALL, &notifyIconData.hIcon);
    notifyIconData.hWnd = hWnd;
    notifyIconData.uID = 0;
    notifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    notifyIconData.uCallbackMessage = WM_APP_NOTIFY_ICON;
    notifyIconData.uVersion = NOTIFYICON_VERSION_4;
    notifyIconData.dwInfoFlags = NIIF_INFO;

    _stprintf_s(notifyIconData.szTip, _T("WinK760"));
    _stprintf_s(notifyIconData.szInfoTitle, _T("WinK760"));

    Shell_NotifyIcon(NIM_ADD, &notifyIconData);
}

NotifyIcon::~NotifyIcon(void)
{
    Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
    DestroyIcon(notifyIconData.hIcon);
}

void NotifyIcon::Balloon(const TCHAR* info)
{
    
    notifyIconData.uFlags |= NIF_INFO;
    _stprintf_s(notifyIconData.szInfo, info);
    Shell_NotifyIcon(NIM_MODIFY, &notifyIconData);
}
