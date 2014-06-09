//
// WinK760: A utility for enabling Fn-key inversion for the Logitech K760 keyboard on Windows.
//
// WinK760 is inspired by the Solaar and UPower libraries.
//
// Copyright 2013-2014 Jesper Hellesø Hansen

#include "Window.h"
#include "stdafx.h"
#include <commctrl.h>

#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "Bthprops.lib")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE,
                       _In_ LPTSTR    lpCmdLine,
                       _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    InitCommonControls();

    Window window;
    window.Register(hInstance);

    if (!window.Initialize(hInstance, &window))
    {
        return 0;
    }

    RAWINPUTDEVICE device;
    device.usUsagePage = 0x00C;
    device.usUsage = 0x0001;
    device.dwFlags = RIDEV_INPUTSINK;
    device.hwndTarget = window.GetHWND();
    RegisterRawInputDevices(&device, 1, sizeof(RAWINPUTDEVICE));

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

