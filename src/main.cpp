//
// WinK760: A utility for enabling Fn-key inversion for the Logitech K760 keyboard on Windows.
//
// WinK760 is inspired by the Solaar and UPower libraries.
//
// Copyright 2013-2014 Jesper Hellesoe Hansen

#include "stdafx.h"
#include "window.h"
#include "mingw-unicode-gui.h"

#ifdef _MSC_VER
#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "Bthprops.lib")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif // _MSC_VER

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE /*hOldInstance*/,
                       _In_ LPTSTR    /*lpCmdLine*/,
                       _In_ int       /*nCmdShow*/)
{
    InitCommonControls();

    Window window;
    window.Register(hInstance);

    if (!window.Initialize(hInstance, &window))
    {
        return 0;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}
