#pragma once

#include <sal.h>
#include <SDKDDKVer.h>
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <stdio.h>
#include <shellapi.h>
#include <bthdef.h>
#include <stdlib.h>
#include <utility>
#include <process.h>
#include <commctrl.h>
#include <Dbt.h>
#include <BluetoothAPIs.h>
#include <Setupapi.h>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);   \
    void operator=(const TypeName&)
