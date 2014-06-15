#pragma once

#define WM_APP_NOTIFY_ICON (WM_APP + 1)

class NotifyIcon {
public:
    NotifyIcon(HINSTANCE hInstance, HWND hWnd);
    ~NotifyIcon(void);
    void Balloon(const TCHAR* info);

private:
    NOTIFYICONDATA notifyIconData;

    DISALLOW_COPY_AND_ASSIGN(NotifyIcon);
};