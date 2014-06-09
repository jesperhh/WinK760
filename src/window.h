#pragma once

#include <windows.h>
#include <memory>

class NotifyIcon;
class BluetoothMonitor;
class Worker;

class Window
{
public:
    Window(void);
    ~Window(void);

    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

    LRESULT CALLBACK OnRawInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    ATOM Register(HINSTANCE hInstance);
    bool Initialize(HINSTANCE hInstance, Window* window);

    HWND GetHWND(void) { return hwnd; }

private:
    HWND hwnd;
    HINSTANCE hInstance;
    HMENU contextMenu;
    HICON icon;
    RECT windowRect;

    std::unique_ptr<NotifyIcon> notifyIcon;
    std::unique_ptr<BluetoothMonitor> monitor;
    std::unique_ptr<Worker> worker;

    void OnPaint(HWND hWnd);
    LRESULT OnDeviceChange(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK OnNotifyIcon(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

