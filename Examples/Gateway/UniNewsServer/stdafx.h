//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Server |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//---
#define WINVER               0x0601          // Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINNT         0x0601          // Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINDOWS       0x0601          // Change this to the appropriate value to target Windows Me or later.
#define _WIN32_IE            0x0601          // Change this to the appropriate value to target other versions of IE.
//---
#include <stdio.h>
#include <stddef.h>
#include <malloc.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <new.h>
#include <wchar.h>
#include <process.h>
#include <Winsock2.h>
//--- resources
#include "resource.h"
//--- for using MT5 tools
#include "..\..\..\Include\MT5APIGateway.h"
//--- tools
#include "Tools\MIMEConverter.h"
#include "Tools\SHA256.h"
//--- common
#include "Common\const.h"
#include "Common\Logger.h"
//--- universal news protocol
#include "UniNewsAPI\Common\UniNewsProtocol.h"
//--- news data source
#include "UniNewsAPI\Bases\UniNewsSource.h"
//--- universal news server port
#include "UniNewsAPI\Contexts\UniNewsServerPort.h"
//+------------------------------------------------------------------+
