#include "bluetooth.h"

#include <iostream>
#include <Dbt.h>
#include <BluetoothAPIs.h>
#include <Setupapi.h>


struct flag_type {
    ULONG flag;
    char* name;
};

struct flag_type flags[] = {
    {BDIF_ADDRESS           , "ADDRESS"           },
    {BDIF_COD               , "COD"               },
    {BDIF_NAME              , "NAME"              },
    {BDIF_PAIRED            , "PAIRED"            },
    {BDIF_PERSONAL          , "PERSONAL"          },
    {BDIF_CONNECTED         , "CONNECTED"         },
    {BDIF_SSP_SUPPORTED     , "SSP_SUPPORTED"     },
    {BDIF_SSP_PAIRED        , "SSP_PAIRED"        },
    {BDIF_SSP_MITM_PROTECTED, "SSP_MITM_PROTECTED"},
};

#define n_flags (sizeof(flags) / sizeof(flags[0]))

void print_flags(const BTH_RADIO_IN_RANGE * radio_in_range)
{
    for (int i = 0; i< n_flags; i++)
    {
        if ((radio_in_range->deviceInfo.flags & flags[i].flag) == flags[i].flag)
        {
            std::cout << "Flag: " << flags[i].name << std::endl;
        }
    }
}

BTH_RADIO_IN_RANGE* BluetoothMonitor::TranslateMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!(message == WM_DEVICECHANGE && wParam == DBT_CUSTOMEVENT))
        return false;

    DEV_BROADCAST_HDR* header = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);
    if (header->dbch_devicetype != DBT_DEVTYP_HANDLE)
        return false;

    DEV_BROADCAST_HANDLE* handle = reinterpret_cast<DEV_BROADCAST_HANDLE*>(header);
    if (!IsEqualGUID(handle->dbch_eventguid, GUID_BLUETOOTH_RADIO_IN_RANGE))
        return false;

    BTH_RADIO_IN_RANGE* radio_in_range = reinterpret_cast<BTH_RADIO_IN_RANGE*>(handle->dbch_data);
    if (0 != strcmp(deviceName, radio_in_range->deviceInfo.name))
        return false;

    return radio_in_range;
}

BluetoothMonitor::BluetoothMonitor(const char* deviceName, HWND hwnd): notify(nullptr)
{
    strncpy_s(this->deviceName, deviceName, strlen(deviceName));

    BLUETOOTH_FIND_RADIO_PARAMS radios = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS)};
    HANDLE handle;
    HBLUETOOTH_RADIO_FIND radios_find = BluetoothFindFirstRadio(&radios, &handle);
    if (radios_find == nullptr)
        return;
    /*while (true)
    {
        std::cout << "radio " << radios_find;*/
    
    DEV_BROADCAST_HANDLE dbh;
    ZeroMemory(&dbh, sizeof(dbh));
    dbh.dbch_devicetype = DBT_DEVTYP_HANDLE;
    dbh.dbch_size = sizeof(dbh);
    dbh.dbch_handle = handle;
    dbh.dbch_eventguid = GUID_BLUETOOTH_RADIO_IN_RANGE;
    notify = RegisterDeviceNotification(hwnd, &dbh, DEVICE_NOTIFY_WINDOW_HANDLE);

    /*    if (!BluetoothFindNextRadio(radios_find, &handle))
            break;
    }*/

    BluetoothFindRadioClose(radios_find);
}

BluetoothMonitor::~BluetoothMonitor(void)
{
    if (notify)
    {
        UnregisterDeviceNotification(notify);
    }
}
