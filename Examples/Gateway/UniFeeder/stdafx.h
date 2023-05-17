//+------------------------------------------------------------------+
//|                                           MetaTrader 5 UniFeeder |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//--- versions
#define WINVER               0x0601          // Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINNT         0x0601          // Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINDOWS       0x0601          // Change this to the appropriate value to target Windows Me or later.
#define _WIN32_IE            0x0601          // Change this to the appropriate value to target other versions of IE.
//--- standard inclusions
#include <stdio.h>
#include <tchar.h>
#include <stddef.h>
#include <time.h>
#include <malloc.h>
#include <new.h>
#include <Winsock2.h>
//--- API
#include "..\..\..\Include\MT5APIGateway.h"
//--- common
#include "Common\const.h"
#include "Common\Logger.h"
//+------------------------------------------------------------------+
