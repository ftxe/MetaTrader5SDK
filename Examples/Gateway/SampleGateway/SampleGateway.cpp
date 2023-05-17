//+------------------------------------------------------------------+
//|                                      MetaTrader 5 Sample Gateway |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "MTGatewayApp.h"
//+------------------------------------------------------------------+
//| Entry point                                                      |
//+------------------------------------------------------------------+
int wmain(int argc,wchar_t** argv)
  {
//--- display banner
   wprintf_s(L"%s %u bit build %d (%s)\n"
             L"Copyright 2000-2023, MetaQuotes Ltd.\n",
             ProgramName,(UINT)(sizeof(char*)*8),ProgramBuild,ProgramBuildDate);
//--- initialize application
   CMTGatewayApp app;
   if(!app.Initialize(argc,argv))
      return(-1);
//--- start application
   app.Run();
//--- shutdown application
   app.Shutdown();
//--- exit
   return(0);
  }
//+------------------------------------------------------------------+
