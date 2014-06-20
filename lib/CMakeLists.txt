add_library(hidapi
    STATIC
    hidapi/hidapi/hidapi.h
    hidapi/windows/hid.c
)

set_target_properties(hidapi PROPERTIES COMPILE_DEFINITIONS "WIN32_LEAN_AND_MEAN;WIN32_EXTRA_LEAN;_UNICODE;UNICODE;_CRT_SECURE_NO_WARNINGS")
target_include_directories(hidapi PUBLIC hidapi/hidapi)