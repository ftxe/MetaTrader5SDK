//+------------------------------------------------------------------+
//|                           MetaTrader 5 Accounts.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "Reports\AccountsGroups.h"
#include "Reports\AccountsGrowth.h"
#include "Tools\ReportFactory.h"
//+------------------------------------------------------------------+
//| Report factories array                                           |
//+------------------------------------------------------------------+
static const CReportFactory g_factories[]=
{
   CReportFactory::Create<CAccountsGroups>(),
   CReportFactory::Create<CAccountsGrowth>(),
   };
//+------------------------------------------------------------------+
//| DLL Main Function                                                |
//+------------------------------------------------------------------+
BOOL APIENTRY DllMain(HMODULE hModule,DWORD reason,LPVOID lpReserved)
  {
//---
   switch(reason)
     {
      case DLL_PROCESS_ATTACH:
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
      case DLL_PROCESS_DETACH:
         break;
     }
   return(TRUE);
  }
//+------------------------------------------------------------------+
//| Plug-in About entry function                                     |
//+------------------------------------------------------------------+
MTAPIENTRY MTAPIRES MTReportAbout(const UINT index,MTReportInfo& info)
  {
//--- check index
   if(index>=_countof(g_factories))
      return(MT_RET_ERR_NOTFOUND);
//--- report information
   return(g_factories[index].About(info));
  }
//+------------------------------------------------------------------+
//| Plug-in instance creation entry point                            |
//+------------------------------------------------------------------+
MTAPIENTRY MTAPIRES MTReportCreate(const UINT index,const UINT apiversion,IMTReportContext **context)
  {
//--- check index
   if(index>=_countof(g_factories))
      return(MT_RET_ERR_NOTFOUND);
//--- create report instance
   return(g_factories[index].Create(context));
  }
//+------------------------------------------------------------------+
