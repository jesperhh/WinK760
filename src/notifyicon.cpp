#include "stdafx.h"
#include "notifyicon.h"
#include "res/resource.h"

#ifdef __MINGW32__
enum _LI_METRIC
{
    LIM_SMALL, // corresponds to SM_CXSMICON/SM_CYSMICON
    LIM_LARGE  // corresponds to SM_CXICON/SM_CYICON
};
#endif // !LIM_SMALL

NotifyIcon::NotifyIcon(HINSTANCE hInstance, HWND hWnd)
{
    ZeroMemory(&this->notifyIconData, sizeof(NOTIFYICONDATA));

    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
#ifdef __MINGW32__
    notifyIconData.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
#else
    LoadIconMetric(hInstance, MAKEINTRESOURCE(IDI_ICON), LIM_SMALL, &notifyIconData.hIcon);
#endif
    notifyIconData.hWnd = hWnd;
    notifyIconData.uID = 0;
    notifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    notifyIconData.uCallbackMessage = WM_APP_NOTIFY_ICON;
    notifyIconData.uVersion = NOTIFYICON_VERSION;
    notifyIconData.dwInfoFlags = NIIF_INFO;

    #ifdef __MINGW32__
    ::swprintf(notifyIconData.szTip, _T("WinK760"));
    ::swprintf(notifyIconData.szInfoTitle, _T("WinK760"));
    #else
    ::swprintf(notifyIconData.szTip, 128, _T("WinK760"));
    ::swprintf(notifyIconData.szInfoTitle, 64, _T("WinK760"));
    #endif // !LIM_SMALL

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
#ifdef __MINGW32__
    ::swprintf(notifyIconData.szInfo, info);
#else
    ::swprintf(notifyIconData.szInfo, 256, info);
#endif // !LIM_SMALL
    Shell_NotifyIcon(NIM_MODIFY, &notifyIconData);
}
