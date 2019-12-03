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

VOID CALLBACK TimerProc(
    _In_  HWND /*hwnd*/,
    _In_  UINT /*uMsg*/,
    _In_  UINT_PTR idEvent,
    _In_  DWORD /*dwTime*/
    )
{
    EjectKey* ejectKey = reinterpret_cast<EjectKey*>(idEvent);
    ejectKey->RepeatKey();
}

EjectKey::EjectKey(HWND hWnd)
{
    this->m_hwnd = hWnd;

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
    GetRemappedScanCode(extendedScanCode, m_scanCode);
    BYTE key = static_cast<BYTE>(MapVirtualKey(m_scanCode, MAPVK_VSC_TO_VK));

    // Read keyboard repeat delay and rate for eject key remap
    SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &m_delay, 0);
    SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &m_rate, 0);

    // Store eject key remap and delay/rate info
    m_flags = extendedScanCode == 0 ? 0 : KEYEVENTF_EXTENDEDKEY;
    m_scanCode = m_scanCode;
    m_rate = 400 - (WORD)(((float)(m_rate)) * 11.5f);
    m_delay = (m_delay + 1) * 250;
    m_virtualKey = key;
}

EjectKey::~EjectKey(void)
{
    KillTimer(this->m_hwnd, reinterpret_cast<UINT_PTR>(this));
}

void EjectKey::HandleRawInput(const LPARAM &lParam, const HWND &hWnd)
{
    // Determine size of raw input data
    UINT dwSize = 0;
    if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER)) == (UINT)-1 || !dwSize)
        return;

    // Allocate enough room for input data and read it
    std::unique_ptr<BYTE[]> lpb(new BYTE[dwSize]);
    if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb.get(), &dwSize, sizeof(RAWINPUTHEADER)) == (UINT)-1 || !dwSize)
        return;

    // Get actual HID report
    RAWINPUT* rawinput = reinterpret_cast<RAWINPUT*>(lpb.get());
    RAWHID* rawhid = &(rawinput)->data.hid;
    if (!rawhid)
        return;

    uint32_t* report = reinterpret_cast<uint32_t*>(rawhid->bRawData);

    if (rawhid->dwSizeHid == 4 && report)
    {
        // We may receive more than one report from each VM_INPUT, loop  through them
        for (unsigned int p = 0; p<rawhid->dwCount; p++)
        {
            // Eject down
            if (0x00200002 == *report)
            {
                SendKeyPress(false);
                SetTimer(hWnd, reinterpret_cast<UINT_PTR>(this), m_delay, TimerProc);

            }
            // Eject up
            else if (0x00000002 == *report)
            {
                SendKeyPress(true);
                KillTimer(hWnd, reinterpret_cast<UINT_PTR>(this));
            }
            report++;
        }
    }
}

LRESULT CALLBACK EjectKey::OnRawInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HandleRawInput(lParam, hWnd);

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void EjectKey::RepeatKey()
{
    SendKeyPress(false);
    SendKeyPress(true);
    SetTimer(m_hwnd, reinterpret_cast<UINT_PTR>(this), m_rate, TimerProc);
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

void EjectKey::SendKeyPress(bool keyUp)
{
    INPUT input = { 0 };

    input.type = INPUT_KEYBOARD;
    input.ki.wVk = m_virtualKey;
    input.ki.wScan = m_scanCode;
    input.ki.dwFlags = m_flags;
    
    if (keyUp)
    {
        input.ki.dwFlags |= KEYEVENTF_KEYUP;
    }

    SendInput(1, &input, sizeof(INPUT));
}
