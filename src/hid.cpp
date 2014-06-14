#include "stdafx.h"
#include "hid.h"
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

static hid_device *handle = 0;

bool init()
{
	/* Initialize HIDAPI */
	if (hid_init())
		return false;

    /* Enumerate HID device */
    struct hid_device_info *devs, *cur_dev;
	devs = hid_enumerate(LOGITECH_K760_VENDOR_ID, LOGITECH_K760_PRODUCT_ID);
	cur_dev = devs;	

	char* path_to_open = 0;
	while (cur_dev) {
		if (cur_dev->usage_page == 0xff00 && cur_dev->usage == 0x0002)
        {
			path_to_open = cur_dev->path;
            break;
        }

		cur_dev = cur_dev->next;
	}

    if (path_to_open == 0)
        return false;

    /* Open HID device */
	handle = hid_open_path(path_to_open);
	hid_free_enumeration(devs);

    return true;
}

int write_device_cmd(hid_device* device,
					  UINT8 feature_idx, UINT8 function_idx, 
					  UINT8 parm1, UINT8 parm2, UINT8 parm3,
					  UINT8 *response)
{
    /* Prepare command */
	UINT8 buf[HIDPP_RESPONSE_LONG_LENGTH];
	memset(buf, 0, HIDPP_RESPONSE_LONG_LENGTH);
	buf[0] = HIDPP_HEADER_REQUEST;
	buf[1] = HIDPP_DEVICE_BLUETOOTH;
	buf[2] = feature_idx;
	buf[3] = function_idx;
	buf[4] = parm1;
	buf[5] = parm2;
	buf[6] = parm3;

    /* Write command to hid device */
    if (hid_write(device, buf, HIDPP_RESPONSE_LONG_LENGTH) != HIDPP_RESPONSE_LONG_LENGTH)
        return ERROR_RETURN_VAL;

    /* try to read response from hid device */
    if (hid_read_timeout(device, response, HIDPP_RESPONSE_LONG_LENGTH, HIDPP_TIMEOUT) != HIDPP_RESPONSE_LONG_LENGTH)
        return ERROR_RETURN_VAL;

    return 0;
}

void finalize(void)
{
    /* Close hid handle (if it exists) and finalize hidapi library */
    if (handle)
    {
        hid_close(handle);
        handle = 0;
    }

    hid_exit();
}

bool pingKeyboard(void)
{
    UINT8 buf[HIDPP_RESPONSE_LONG_LENGTH];
    UINT8 ping = 0x42;
    int res = write_device_cmd(handle, 
        HIDPP_FEATURE_ROOT_INDEX, 
        HIDPP_FEATURE_ROOT_FN_PING,
        0, 0, ping, buf);
    
    return !(res || buf[4] != 2 || buf[6] != ping);
}

bool setFunctionKeyStatus(bool status)
{
    if (!init())
        return false;

    /* Get feature index */
    UINT8 buf[HIDPP_RESPONSE_LONG_LENGTH];
    int res = write_device_cmd(handle, 
        HIDPP_FEATURE_ROOT_INDEX, 
        HIDPP_FEATURE_ROOT_FN_GET_FEATURE, 
        HIDPP_FN_INVERT_UPPER, 
        HIDPP_FN_INVERT_LOWER, 
        00, buf);

    if (res)
        return false;

    /* Set feature status */
    UINT8 fn_invert_idx = buf[4];
    res = write_device_cmd(handle, 
        fn_invert_idx, 
        HIDPP_FEATURE_STATUS_SET, 
        status ? HIDPP_FEATURE_STATUS_ON : HIDPP_FEATURE_STATUS_OFF,
        0, 0, buf);

    finalize();

	return !res;
}
