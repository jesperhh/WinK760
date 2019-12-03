#pragma once

class EjectKey {
public:
    explicit EjectKey(HWND hWnd);
    ~EjectKey(void);

    LRESULT CALLBACK OnRawInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    void RepeatKey();
private:
    HWND m_hwnd;
    BYTE m_scanCode;
    BYTE m_flags;
    BYTE m_virtualKey;
    DWORD m_rate;
    DWORD m_delay;

    void GetRemappedScanCode(BYTE &extendedScanCode, BYTE &scanCode);

    void HandleRawInput(const LPARAM &lParam, const HWND &hWnd);

    void SendKeyPress(bool keyUp);

    EjectKey(EjectKey const &) = delete;
    void operator=(EjectKey const &ejectKey) = delete;
};
