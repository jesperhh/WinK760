#pragma once

#include <cstdio>
#include <cwchar>
#include <cstdint>
#include <sdkddkver.h>
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <shellapi.h>
#include <bthdef.h>
#include <cstdlib>
#include <utility>
#include <process.h>
#include <commctrl.h>
#include <dbt.h>
#include <bluetoothapis.h>
#include <setupapi.h>
#include <sal.h>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);   \
    void operator=(const TypeName&)
