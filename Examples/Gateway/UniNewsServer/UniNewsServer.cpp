//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Server |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "MTUniNewsServerApp.h"
//+------------------------------------------------------------------+
//| Entry point                                                      |
//+------------------------------------------------------------------+
int wmain(int argc,wchar_t** argv)
  {
//--- display banner
   wprintf_s(L"%s %u bit build %d (%s)\n"
             L"Copyright 2000-2023, MetaQuotes Ltd.\n\n",
             ProgramName,(UINT)(sizeof(char*)*8),ProgramBuild,ProgramBuildDate);
//--- initialize application
   CMTUniNewsServerApp app;
   if(!app.Initialize(argc,argv))
      return(-1);
//--- start application
   app.Run();
//--- exit
   return(0);
  }
//+------------------------------------------------------------------+
