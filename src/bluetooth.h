#pragma once

#define MAX_RADIOS (10)

class BluetoothMonitor {
public:
    BluetoothMonitor(const char* deviceName, const HWND hwnd);
    ~BluetoothMonitor(void);
    BTH_RADIO_IN_RANGE* TranslateMessage(const UINT message, const WPARAM wParam, const LPARAM lParam);
private:
    CHAR deviceName[BTH_MAX_NAME_SIZE];
    HDEVNOTIFY notify[MAX_RADIOS];

    DISALLOW_COPY_AND_ASSIGN(BluetoothMonitor);
};