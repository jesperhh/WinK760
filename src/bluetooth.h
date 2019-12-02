#pragma once

#include <string>
#include <vector>

class BluetoothMonitor {
public:
    enum class BluetoothStatus { Unknown, Connected, Disconnected };

    BluetoothMonitor(const std::string& deviceName, const HWND hwnd);
    ~BluetoothMonitor(void);
    BluetoothStatus TranslateMessage(const UINT message, const WPARAM wParam, const LPARAM lParam);

private:
    std::string m_deviceName;
    std::vector<HDEVNOTIFY> m_notificationHandles;

    BluetoothMonitor(BluetoothMonitor const &) = delete;
    void operator=(BluetoothMonitor const &bluetoothMonitor) = delete;
};