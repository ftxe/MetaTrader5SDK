//+------------------------------------------------------------------+
//|                          MetaTrader 5 Trades.Transaction.Reports |
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
#include <windows.h>
#include <new.h>
#include <stddef.h>
//--- Report API
#include "..\..\..\Include\MT5APIReport.h"
#include "..\..\..\Include\MT5APIServer.h"
//+------------------------------------------------------------------+
//| Constants                                                        |
//+------------------------------------------------------------------+
#define Copyright L"Copyright 2000-2023, MetaQuotes Ltd."
//+------------------------------------------------------------------+
//| Macros of a size                                                 |
//+------------------------------------------------------------------+
#define KB UINT(1024)
#define MB UINT(1024*1024)
//+------------------------------------------------------------------+
//| Macros for calculation member size                               |
//+------------------------------------------------------------------+
#define MtFieldSize(type,member) (sizeof(((type*)(0))->member))
//+------------------------------------------------------------------+
//| Report column description (description of record field)          |
//+------------------------------------------------------------------+
struct ReportColumn
  {
   UINT              id;               // unique column id (must be greater than 0)
   LPCWSTR           name;             // visible column title
   UINT              type;             // data type and formating of column
   UINT              width;            // relative width
   UINT              width_max;        // max width in pixel
   UINT              offset;           // field offset
   UINT              size;             // size in bytes for strings
   UINT              digits_column;    // id of column with digits value
   UINT64            flags;            // flags
  };
//+------------------------------------------------------------------+
//| Default params                                                   |
//+------------------------------------------------------------------+
#define DEFAULT_BASE_DIRECTORY L"Trade Transaction Report"
#define DEFAULT_SHOW_OWNER     L"Yes"
#define DEFAULT_SHOW_DAILY     L"Yes"
#define DEFAULT_SHOW_REASON    L"No"
#define DEFAULT_SHOW_RETCODE   L"No"
#define DEFAULT_SHOW_REJECTED  L"No"
//+------------------------------------------------------------------+