#pragma once

#include <sal.h>
#include <cstdint>
#include <sdkddkver.h>
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <cstdio>
#include <shellapi.h>
#include <bthdef.h>
#include <stdlib.h>
#include <utility>
#include <process.h>
#include <commctrl.h>
#include <dbt.h>
#include <bluetoothapis.h>
#include <setupapi.h>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);   \
    void operator=(const TypeName&)
