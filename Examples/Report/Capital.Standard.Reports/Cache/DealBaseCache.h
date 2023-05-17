//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportCacheKeysTime.h"
#include "..\Tools\DatasetField.h"
//+------------------------------------------------------------------+
//| Deals cache base class                                           |
//+------------------------------------------------------------------+
class CDealBaseCache : public CReportCacheKeysTime
  {
protected:
   explicit          CDealBaseCache(CReportCacheKeysTimeContext &context) : CReportCacheKeysTime(context) {}
   virtual          ~CDealBaseCache(void)=default;
   //--- write cache range
   virtual MTAPIRES  WriteRange(IMTDatasetRequest &request,INT64 from,INT64 to) override;
   //--- request deals from base
   virtual MTAPIRES  SelectData(IMTDataset &deals,IMTDatasetRequest &request,INT64 from,INT64 to,UINT64 id_from) override;
   //--- fill deals request
   virtual MTAPIRES  DealRequest(IMTDatasetRequest &request,INT64 from,INT64 to,UINT64 id_from)=0;
   //--- end of month
   static INT64      MonthEnd(INT64 ctm,INT64 to);
   //--- calculate upper power of 2
   static UINT       UpperPowerOfTwo(UINT v);
   //--- logging
   virtual MTAPIRES  LogSelectError(const MTAPIRES res,const UINT keys,const INT64 from,const INT64 to,const UINT64 id_from) override
     {
      return(LogErrorTime(res,L"Deals selection for %u accounts from #%I64u with time",keys,from,to,id_from));
     }
   virtual MTAPIRES  LogWriteError(const MTAPIRES res,const INT64 from,const INT64 to) override
     {
      return(LogErrorTime(res,L"Deals aggregation with time",from,to));
     }
   virtual MTAPIRES  LogWriteOk(const INT64 from,const INT64 to,const UINT64 total,const UINT keys,const UINT time_select,const UINT time_write) override
     {
      return(LogOkTime(L"Selected %I64u deals of %u accounts in %u ms, aggregated in %u ms, time",from,to,total,keys,time_select,time_write));
     }
  };
//+------------------------------------------------------------------+
