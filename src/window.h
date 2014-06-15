#pragma once

class NotifyIcon;
class BluetoothMonitor;
class Worker;
class EjectKey;

class Window
{
public:
    Window(void);
    ~Window(void);

    ATOM Register(HINSTANCE hInstance);
    bool Initialize(HINSTANCE hInstance, Window* window);
    HWND GetHWND(void) { return hwnd; }

private:
    HWND hwnd;
    HMENU contextMenu;
    HICON icon;
    RECT windowRect;
    TCHAR aboutText[100];

    std::unique_ptr<BluetoothMonitor> monitor;
    std::unique_ptr<NotifyIcon> notifyIcon;
    std::unique_ptr<Worker> worker;
    std::unique_ptr<EjectKey> ejectKey;

    void OnPaint(HWND hWnd);
    LRESULT OnDeviceChange(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK OnNotifyIcon(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

    DISALLOW_COPY_AND_ASSIGN(Window);
};

