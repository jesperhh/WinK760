#include "stdafx.h"
#include "bluetooth.h"

BluetoothMonitor::BluetoothMonitor(const std::string& deviceName, const HWND hwnd)
{
    m_deviceName = deviceName;
    m_notificationHandles.clear();

    BLUETOOTH_FIND_RADIO_PARAMS find_radios_params = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
    HANDLE handle;
    HBLUETOOTH_RADIO_FIND radios_find = BluetoothFindFirstRadio(&find_radios_params, &handle);
    if (radios_find == nullptr)
    {
        return;
    }

    DEV_BROADCAST_HANDLE dbh = { 0 };
    dbh.dbch_devicetype = DBT_DEVTYP_HANDLE;
    dbh.dbch_size = sizeof(dbh);
    dbh.dbch_eventguid = GUID_BLUETOOTH_RADIO_IN_RANGE;

    do
    {   
        dbh.dbch_handle = handle;
        HDEVNOTIFY notify = RegisterDeviceNotification(hwnd, &dbh, DEVICE_NOTIFY_WINDOW_HANDLE);
        CloseHandle(handle);

        if (notify)
        {
            m_notificationHandles.push_back(notify);
        }
    } 
    while (BluetoothFindNextRadio(radios_find, &handle));

    BluetoothFindRadioClose(radios_find);
}

BluetoothMonitor::~BluetoothMonitor(void)
{
    for (HDEVNOTIFY notify : m_notificationHandles)
    {
        UnregisterDeviceNotification(notify);
    }
}

BluetoothMonitor::BluetoothStatus BluetoothMonitor::TranslateMessage(const UINT message, const WPARAM wParam, const LPARAM lParam)
{
    if (message != WM_DEVICECHANGE || wParam != DBT_CUSTOMEVENT || !lParam)
        return BluetoothStatus::Unknown;

    DEV_BROADCAST_HDR* header = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);
    if (header->dbch_devicetype != DBT_DEVTYP_HANDLE)
        return BluetoothStatus::Unknown;

    DEV_BROADCAST_HANDLE* handle = reinterpret_cast<DEV_BROADCAST_HANDLE*>(header);
    if (!IsEqualGUID(handle->dbch_eventguid, GUID_BLUETOOTH_RADIO_IN_RANGE))
        return BluetoothStatus::Unknown;

    BTH_RADIO_IN_RANGE* radio_in_range = reinterpret_cast<BTH_RADIO_IN_RANGE*>(handle->dbch_data);
    if (!radio_in_range)
        return BluetoothStatus::Unknown;

    if (m_deviceName != radio_in_range->deviceInfo.name)
        return BluetoothStatus::Unknown;
    
    if (radio_in_range->deviceInfo.flags & BDIF_CONNECTED)
        return BluetoothStatus::Connected;

    return BluetoothStatus::Disconnected;
}
