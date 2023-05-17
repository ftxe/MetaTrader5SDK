//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once

#define WINVER               _WIN32_WINNT_WIN7
#define _WIN32_WINNT         _WIN32_WINNT_WIN7
#define _WIN32_WINDOWS       _WIN32_WINNT_WIN7
#define _WIN32_IE            _WIN32_IE_IE90
#define NTDDI_VERSION        NTDDI_WIN7

#define _WINSOCK_DEPRECATED_NO_WARNINGS
//---
#include <float.h>
#include <windows.h>
#include <new.h>
#include <stddef.h>
#include <stdint.h>
//---
#include "resource.h"
//---
#include "..\..\..\Include\MT5APIReport.h"
#include "Tools\ReportColumn.h"
//--- need for timeGetTime func
#pragma comment(lib, "Winmm.lib")
//+------------------------------------------------------------------+
