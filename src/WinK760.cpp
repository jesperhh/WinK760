//
// WinK760: A utility for enabling Fn-key inversion for the Logitech K760 keyboard on Windows.
//
// WinK760 is inspired by the Solaar and UPower libraries.
//
// Copyright 2013 Jesper Hellesø Hansen

#pragma comment(lib, "Setupapi.lib")

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "hidapi.h"

#define LOGITECH_K760_VENDOR_ID                 0x046d
#define LOGITECH_K760_PRODUCT_ID                0xb318
#define HIDPP_RESPONSE_LONG_LENGTH				20
#define HIDPP_DEVICE_BLUETOOTH					0xFF
#define HIDPP_HEADER_REQUEST					0x11
#define HIDPP_FEATURE_ROOT_INDEX				0x00
#define HIDPP_FEATURE_ROOT_FN_GET_FEATURE		(0x00 << 4)
#define HIDPP_FEATURE_ROOT_FN_PING				(0x01 << 4)
#define HIDPP_FEATURE_STATUS_SET                0x10
#define HIDPP_FEATURE_STATUS_OFF                0x0
#define HIDPP_FEATURE_STATUS_ON                 0x1
#define HIDPP_FN_INVERT_UPPER                   0x40
#define HIDPP_FN_INVERT_LOWER                   0xA0
#define HIDPP_TIMEOUT                           8000

#define ERROR_RETURN_VAL                        -1


hid_device* init()
{
	/* Initialize HIDAPI */
	if (hid_init())
		return 0;

    struct hid_device_info *devs, *cur_dev;
	devs = hid_enumerate(LOGITECH_K760_VENDOR_ID, LOGITECH_K760_PRODUCT_ID);
	cur_dev = devs;	

	char* path_to_open = 0;
	while (cur_dev) {
		if (cur_dev->output_length == HIDPP_RESPONSE_LONG_LENGTH)
			path_to_open = cur_dev->path;

		cur_dev = cur_dev->next;
	}

	hid_device *handle = hid_open_path(path_to_open);
	hid_free_enumeration(devs);

	return handle;
}

void print_buf(char* header, UINT8 *buf)
{
#ifdef _DEBUG
    printf("%s", header);
    printf("%s", "\n");
    for (int i = 0; i < HIDPP_RESPONSE_LONG_LENGTH; i++)
        printf("0x%02x ", buf[i]);
    printf("%s", "\n");
#endif
}

int write_device_cmd(hid_device* device,
					  UINT8 feature_idx, UINT8 function_idx, 
					  UINT8 parm1, UINT8 parm2, UINT8 parm3,
					  UINT8 *response)
{
	UINT8 buf[HIDPP_RESPONSE_LONG_LENGTH];
	memset(buf, 0, HIDPP_RESPONSE_LONG_LENGTH);
	buf[0] = HIDPP_HEADER_REQUEST;
	buf[1] = HIDPP_DEVICE_BLUETOOTH;
	buf[2] = feature_idx;
	buf[3] = function_idx;
	buf[4] = parm1;
	buf[5] = parm2;
	buf[6] = parm3;

    if (hid_write(device, buf, HIDPP_RESPONSE_LONG_LENGTH) != HIDPP_RESPONSE_LONG_LENGTH)
        return ERROR_RETURN_VAL;
    if (hid_read_timeout(device, response, HIDPP_RESPONSE_LONG_LENGTH, HIDPP_TIMEOUT) != HIDPP_RESPONSE_LONG_LENGTH)
        return ERROR_RETURN_VAL;

    return 0;
}

void shutdown(const char* cause = 0, hid_device* handle = 0)
{
    if (cause)
        printf("%s\n", cause);

    if (handle)
        hid_close(handle);

    hid_exit();
    system("pause");
}

int _tmain(int argc, _TCHAR* argv[])
{
    printf("Logitech K760 Fn-Lock switcher for Windows\n\n");

    hid_device *handle = init();
	if (!handle)
    {
        shutdown("Could not initialize HIDAPI");
        return 1;
    }

    UINT8 buf[HIDPP_RESPONSE_LONG_LENGTH];
    UINT8 ping = 0x42;
    int res = write_device_cmd(handle, 
        HIDPP_FEATURE_ROOT_INDEX, 
        HIDPP_FEATURE_ROOT_FN_PING,
        0, 0, ping, buf);
    
    if (res || buf[4] != 2 || buf[6] != ping)
    {
        shutdown("Ping failed", handle);
        return 1;
    }

    res = write_device_cmd(handle, 
        HIDPP_FEATURE_ROOT_INDEX, 
        HIDPP_FEATURE_ROOT_FN_GET_FEATURE, 
        HIDPP_FN_INVERT_UPPER, 
        HIDPP_FN_INVERT_LOWER, 
        00, buf);

    if (res)
    {
        shutdown("Get feature failed", handle);
        return 1;
    }

    UINT8 fn_invert_idx = buf[4];

    write_device_cmd(handle, 
        fn_invert_idx, 
        HIDPP_FEATURE_STATUS_SET, 
        HIDPP_FEATURE_STATUS_ON,
        0, 0, buf);

    if (res)
    {
        shutdown("Feature set failed", handle);
        return 1;
    }

	shutdown("Operation completed succesfully", handle);
	return 0;
}