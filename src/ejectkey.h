#pragma once

class EjectKey {
public:
    explicit EjectKey(HWND hWnd);
    ~EjectKey(void);

    LRESULT CALLBACK OnRawInput(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
    HWND hwnd;

    void GetRemappedScanCode(BYTE &extendedScanCode, BYTE &scanCode);

    void HandleRawInput(const LPARAM &lParam, const HWND &hWnd);

    EjectKey(EjectKey const &) = delete;
    void operator=(EjectKey const &ejectKey) = delete;
};