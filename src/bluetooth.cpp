#include "stdafx.h"
#include "bluetooth.h"

BluetoothMonitor::BluetoothMonitor(const char* _deviceName, const HWND hwnd)
{
    strncpy_s(this->deviceName, BTH_MAX_NAME_SIZE, _deviceName, strlen(_deviceName));

    BLUETOOTH_FIND_RADIO_PARAMS find_radios_params = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
    HANDLE handle;
    HBLUETOOTH_RADIO_FIND radios_find = BluetoothFindFirstRadio(&find_radios_params, &handle);
    if (radios_find == nullptr)
    {
        return;
    }

    ZeroMemory(notify, sizeof(notify));

    DEV_BROADCAST_HANDLE dbh;
    ZeroMemory(&dbh, sizeof(dbh));
    dbh.dbch_devicetype = DBT_DEVTYP_HANDLE;
    dbh.dbch_size = sizeof(dbh);
    dbh.dbch_eventguid = GUID_BLUETOOTH_RADIO_IN_RANGE;

    int current_radio = 0;
    for (;;)
    {   
        dbh.dbch_handle = handle;
        notify[current_radio++] = RegisterDeviceNotification(hwnd, &dbh, DEVICE_NOTIFY_WINDOW_HANDLE);
        CloseHandle(handle);

        if (!BluetoothFindNextRadio(radios_find, &handle))
            break;
    }

    BluetoothFindRadioClose(radios_find);
}

BluetoothMonitor::~BluetoothMonitor(void)
{
    for (int i = 0; i<MAX_RADIOS; i++)
    {
        if (notify[i])
        {
            UnregisterDeviceNotification(notify[i]);
        }
        else
        {
            break;
        }
    }
}

BTH_RADIO_IN_RANGE* BluetoothMonitor::TranslateMessage(const UINT message, const WPARAM wParam, const LPARAM lParam)
{
    if (!(message == WM_DEVICECHANGE && wParam == DBT_CUSTOMEVENT))
        return nullptr;

    DEV_BROADCAST_HDR* header = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);
    if (header->dbch_devicetype != DBT_DEVTYP_HANDLE)
        return nullptr;

    DEV_BROADCAST_HANDLE* handle = reinterpret_cast<DEV_BROADCAST_HANDLE*>(header);
    if (!IsEqualGUID(handle->dbch_eventguid, GUID_BLUETOOTH_RADIO_IN_RANGE))
        return nullptr;

    BTH_RADIO_IN_RANGE* radio_in_range = reinterpret_cast<BTH_RADIO_IN_RANGE*>(handle->dbch_data);
    if (0 != strcmp(deviceName, radio_in_range->deviceInfo.name))
        return nullptr;

    return radio_in_range;
}