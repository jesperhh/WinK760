#include "stdafx.h"
#include "ejectkey.h"

struct mapping {
    BYTE toScanCode;
    BYTE toExtScanCode;
    BYTE fromScanCode;
    BYTE fromExtScanCode;
};

struct scancode_map {
    DWORD version;
    DWORD flags;
    DWORD no_of_mappings;
    mapping mappings[1];
};

struct timer_params
{
    BYTE scanCode;
    BYTE flags;
    BYTE virtualKey;
    DWORD rate;
    DWORD delay;
};

#ifndef MAPVK_VSC_TO_VK
    #define MAPVK_VSC_TO_VK 1
#endif

void SendKeyPress(timer_params* params, bool keyUp);

static timer_params timer_param;

VOID CALLBACK TimerProc(
    _In_  HWND hwnd,
    _In_  UINT /*uMsg*/,
    _In_  UINT_PTR idEvent,
    _In_  DWORD /*dwTime*/
    )
{
    timer_params* params = reinterpret_cast<timer_params*>(idEvent);
    SendKeyPress(params, false);
    SendKeyPress(params, true);
    SetTimer(hwnd, idEvent, params->rate, TimerProc);
}

EjectKey::EjectKey(HWND hWnd)
{
    this->hwnd = hWnd;

    // Register that we want raw input for usage page 0x000C/0x0001 - here
    // the "eject" key sends its input
    RAWINPUTDEVICE device = { 0 };
    device.usUsagePage = 0x000C;
    device.usUsage = 0x0001;
    device.dwFlags = RIDEV_INPUTSINK;
    device.hwndTarget = hWnd;
    RegisterRawInputDevices(&device, 1, sizeof(RAWINPUTDEVICE));

    // Send F13 (0x0064 unless remapped via scancode map in registry
    BYTE extendedScanCode = 0x00;
    BYTE scanCode = 0x64;
    GetRemappedScanCode(extendedScanCode, scanCode);
    BYTE key = static_cast<BYTE>(MapVirtualKey(scanCode, MAPVK_VSC_TO_VK));

    // Read keyboard repeat delay and rate for eject key remap
    DWORD delay = 0, rate = 0;
    SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &delay, 0);
    SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &rate, 0);

    // Store eject key remap and delay/rate info
    timer_param.flags = extendedScanCode == 0 ? 0 : KEYEVENTF_EXTENDEDKEY;
    timer_param.scanCode = scanCode;
    timer_param.rate = 400 - (WORD)(((float)(rate)) * 11.5f);
    timer_param.delay = (delay + 1) * 250;
    timer_param.virtualKey = key;
}

EjectKey::~EjectKey(void)
{
    KillTimer(this->hwnd, reinterpret_cast<UINT_PTR>(&timer_param));
}

LRESULT CALLBACK EjectKey::OnRawInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
        for (unsigned int p = 0; p<rawhid->dwCount; p++)
        {
            // Eject down
            if (0x00200002 ==  *report)
            {
                SendKeyPress(&timer_param, false);
                SetTimer(hWnd, reinterpret_cast<UINT_PTR>(&timer_param), timer_param.delay, TimerProc);

            }
            // Eject up
            else if (0x00000002 == *report)
            {
                SendKeyPress(&timer_param, true);
                KillTimer(hWnd, reinterpret_cast<UINT_PTR>(&timer_param));
            }
            report++;
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void EjectKey::GetRemappedScanCode(BYTE &extendedScanCode, BYTE &scanCode)
{
    DWORD regSize = 0;
    if (ERROR_SUCCESS == 
        RegGetValue(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"), 
        _T("Scancode Map"), RRF_RT_REG_BINARY, nullptr, nullptr, &regSize))
    {
        std::unique_ptr<BYTE[]> regkey(new BYTE[regSize]);
        if (ERROR_SUCCESS == RegGetValue(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"), 
            _T("Scancode Map"), RRF_RT_REG_BINARY, nullptr, regkey.get(), &regSize))
        {
            scancode_map* sc_map = reinterpret_cast<scancode_map*>(regkey.get());
            for (unsigned int i = 0; i< sc_map->no_of_mappings; i++)
            {
                mapping& current = sc_map->mappings[i];
                if (current.fromExtScanCode == extendedScanCode && current.fromScanCode == scanCode)
                {
                    extendedScanCode = current.toExtScanCode;
                    scanCode = current.toScanCode;
                    break;
                }
            }
        }
    }
}

void SendKeyPress(timer_params* params, bool keyUp)
{
    INPUT input = { 0 };

    input.type = INPUT_KEYBOARD;
    input.ki.wVk = params->virtualKey;
    input.ki.wScan = params->scanCode;
    input.ki.dwFlags = params->flags;
    
    if (keyUp)
    {
        input.ki.dwFlags |= KEYEVENTF_KEYUP;
    }

    SendInput(1, &input, sizeof(INPUT));
}
