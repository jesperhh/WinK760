#pragma once

#define WM_APP_NOTIFY_ICON (WM_APP + 1)

#include <string>

class NotifyIcon {
public:
    NotifyIcon(HINSTANCE hInstance, HWND hWnd);
    ~NotifyIcon(void);
    void Balloon(const std::wstring& info);

private:
    NOTIFYICONDATA m_notifyIconData = { 0 };

    NotifyIcon(NotifyIcon const &) = delete;
    void operator=(NotifyIcon const &notifyIcon) = delete;
};