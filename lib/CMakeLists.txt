add_library(hidapi
	STATIC
    hidapi/hidapi/hidapi.h
    hidapi/windows/hid.c
)

target_include_directories(hidapi PUBLIC hidapi/hidapi)