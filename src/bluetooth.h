#pragma once
#include <Windows.h>
#include <Bthdef.h>

class BluetoothMonitor {
public:
    BluetoothMonitor(const char* deviceName, HWND hwnd);
    ~BluetoothMonitor(void);
    BTH_RADIO_IN_RANGE* TranslateMessage(UINT message, WPARAM wParam, LPARAM lParam);
private:
    CHAR deviceName[248];
    HDEVNOTIFY notify;
};