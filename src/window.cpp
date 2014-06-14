#include "stdafx.h"
#include "window.h"
#include "notifyicon.h"
#include "bluetooth.h"
#include "res/resource.h"
#include "worker.h"

Window::Window(void): hwnd(nullptr), contextMenu(nullptr), icon(nullptr), monitor(), notifyIcon()
{
    contextMenu = CreatePopupMenu();
    AppendMenu(contextMenu, MF_STRING, ID_TRAY_CONTEXT_SHOW, _T("Restore"));
    AppendMenu(contextMenu, MF_STRING, ID_TRAY_CONTEXT_EXIT, _T("Exit"));
}

Window::~Window(void)
{
    DestroyMenu(contextMenu);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SYSCOMMAND:
        switch (wParam)
        {
        case SC_MINIMIZE:
        case SC_CLOSE:
            ShowWindow(hWnd, SW_HIDE);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    case WM_NCHITTEST:
        {
            LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
            return hit == HTCLIENT ? HTCAPTION : hit;
        }
    case WM_APP_NOTIFY_ICON:
        return OnNotifyIcon(hWnd, message, wParam, lParam);
    case WM_DEVICECHANGE:
        return OnDeviceChange(hWnd, message, wParam, lParam);
    case WM_INPUT:
        return OnRawInput(hWnd, message, wParam, lParam);
    case WM_PAINT:
        OnPaint(hWnd);
        break;
    case WM_CLOSE:
        ShowWindow(hWnd, SW_HIDE);
        break;
    case WM_DESTROY:
        worker->Exit();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK Window::StaticWndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window *self;
    if (uMsg == WM_NCCREATE) 
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        self = reinterpret_cast<Window *>(lpcs->lpCreateParams);
        self->hwnd = hwnd;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(self));
    } 
    else 
    {
        self = reinterpret_cast<Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (self) 
    {
        return self->WndProc(hwnd, uMsg, wParam, lParam);
    }
    else 
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

ATOM Window::Register(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= Window::StaticWndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= sizeof(Window*);
    wcex.hInstance		= hInstance; 
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= nullptr;
    wcex.lpszClassName	= _T("WINK760");
    wcex.hIconSm		= nullptr;

    return RegisterClassEx(&wcex);
}

bool Window::Initialize(HINSTANCE hInstance, Window* window)
{
    DWORD style = WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN |WS_CLIPSIBLINGS;
    DWORD exStyle = WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE;

    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = 256 + 10;
    windowRect.bottom = 256 - 30;
    AdjustWindowRectEx(&windowRect, style, false, exStyle);
    OffsetRect(&windowRect, -windowRect.left, -windowRect.top);

    HWND hWnd = CreateWindowEx(exStyle, _T("WINK760"), _T("Wink760"), style,
        CW_USEDEFAULT, 0, windowRect.right , windowRect.bottom, NULL, NULL, hInstance, window);

    windowRect.right = 256 + 10;
    windowRect.bottom = 256;

    if (!hWnd)
    {
        return false;
    }

    // Register that we want raw input for usage page 0x000C/0x0001 - here
    // the "eject" key sends its input
    RAWINPUTDEVICE device;
    device.usUsagePage = 0x000C;
    device.usUsage = 0x0001;
    device.dwFlags = RIDEV_INPUTSINK;
    device.hwndTarget = hWnd;
    RegisterRawInputDevices(&device, 1, sizeof(RAWINPUTDEVICE));

    this->icon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 256, 256, LR_DEFAULTCOLOR));
    LoadString(hInstance, IDS_ABOUT, aboutText, sizeof(aboutText)/sizeof(aboutText[0]));

    notifyIcon.reset(new NotifyIcon(hInstance, hwnd));
    monitor.reset(new BluetoothMonitor("Logitech K760", hwnd));
    worker.reset(new Worker());
    return true;
}

void Window::OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    HGDIOBJ font = GetStockObject(DEFAULT_GUI_FONT);
    font = SelectObject(hdc, font);

    RECT rect;
    GetClientRect(hWnd, &rect);
    DrawIconEx(hdc, 5, -40,icon, 256, 256, 0, nullptr, DI_NORMAL);
    rect.top = 170;
    rect.left = 0;
    rect.bottom = windowRect.bottom;
    rect.right = windowRect.right;

    DrawText(hdc, aboutText,  -1, &rect, DT_CENTER | DT_TOP);
    SelectObject(hdc, font);
    EndPaint(hWnd, &ps);
}

LRESULT CALLBACK Window::OnNotifyIcon(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WORD event = LOWORD(lParam);
    POINT cursorPosition;
    BOOL clicked;

    switch (event)
    {
    case WM_LBUTTONUP:
        ShowWindow(hWnd, SW_SHOW);
        break;
    case WM_RBUTTONUP:
        GetCursorPos(&cursorPosition);
        SetForegroundWindow(hwnd); 
        clicked = TrackPopupMenu(contextMenu, TPM_RETURNCMD | TPM_NONOTIFY,
            cursorPosition.x, cursorPosition.y, 0, hwnd, NULL);

        if (clicked == ID_TRAY_CONTEXT_EXIT)
        {
            DestroyWindow(hwnd);
        }
        else if (clicked == ID_TRAY_CONTEXT_SHOW)
        {
            ShowWindow(hWnd, SW_SHOW);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

LRESULT Window::OnDeviceChange(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Verify if WM_DEVICE_CHANGE was a bluetooth connect/disconnect message
    BTH_RADIO_IN_RANGE* radio_in_range = monitor->TranslateMessage(message, wParam, lParam);
    if (radio_in_range)
    {
        if (radio_in_range->deviceInfo.flags & BDIF_CONNECTED)
        {
            notifyIcon->Balloon(_T("Connected"));
            worker->Work();
        }
        else
        {
            notifyIcon->Balloon(_T("Disconnected"));
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK Window::OnRawInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Determine size of raw input data
    UINT dwSize = 0;
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

    // Allocate enough room for input data and read it
    std::unique_ptr<BYTE[]> lpb(new BYTE[dwSize]);
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb.get(), &dwSize, sizeof(RAWINPUTHEADER));

    // Get actual HID report
    RAWINPUT* rawinput = reinterpret_cast<RAWINPUT*>(lpb.get());
    RAWHID* rawhid = &(rawinput)->data.hid;
    uint32_t* report = reinterpret_cast<uint32_t*>(rawhid->bRawData);

    if (rawhid->dwSizeHid == 4)
    {
        // We may receive more than one report from each VM_INPUT, loop  through them
        BYTE scanCode = static_cast<BYTE>(MapVirtualKey(VK_F13, MAPVK_VK_TO_VSC));
        for (unsigned int p = 0; p<rawhid->dwCount; p++)
        {
            // Eject down
            if (0x00200002 ==  *report)
            {
                keybd_event(VK_F13, scanCode, 0, 0);
            }
            // Eject up
            else if (0x00000002 == *report)
            {
                keybd_event(VK_F13, scanCode, KEYEVENTF_KEYUP, 0);
            }
            report++;
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
