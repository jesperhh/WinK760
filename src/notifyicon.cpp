#include "stdafx.h"
#include "notifyicon.h"
#include "res/resource.h"

NotifyIcon::NotifyIcon(HINSTANCE hInstance, HWND hWnd)
{
    m_notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    LoadIconMetric(hInstance, MAKEINTRESOURCE(IDI_ICON), LIM_SMALL, &m_notifyIconData.hIcon);
    m_notifyIconData.hWnd = hWnd;
    m_notifyIconData.uID = 0;
    m_notifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_notifyIconData.uCallbackMessage = WM_APP_NOTIFY_ICON;
    m_notifyIconData.uVersion = NOTIFYICON_VERSION;
    m_notifyIconData.dwInfoFlags = NIIF_INFO;

    ::swprintf(m_notifyIconData.szTip, sizeof(m_notifyIconData.szTip), _T("WinK760"));
    ::swprintf(m_notifyIconData.szInfoTitle, sizeof(m_notifyIconData.szInfoTitle), _T("WinK760"));

    Shell_NotifyIcon(NIM_ADD, &m_notifyIconData);
}

NotifyIcon::~NotifyIcon(void)
{
    Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData);
    DestroyIcon(m_notifyIconData.hIcon);
}

void NotifyIcon::Balloon(const std::wstring& info)
{
    m_notifyIconData.uFlags |= NIF_INFO;
    ::swprintf(m_notifyIconData.szInfo, sizeof(m_notifyIconData.szInfo), info.c_str());
    Shell_NotifyIcon(NIM_MODIFY, &m_notifyIconData);
}
