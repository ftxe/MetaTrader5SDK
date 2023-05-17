//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportCache.h"
//+------------------------------------------------------------------+
//| Report cache select base class with time and no keys             |
//+------------------------------------------------------------------+
class CReportCacheTime : public CReportCache
  {
public:
   //--- update cache
   MTAPIRES          Update(const INT64 from,INT64 to);

protected:
   explicit          CReportCacheTime(CReportCacheContext &context) : CReportCache(context) {}
   virtual          ~CReportCacheTime(void)=default;
   //--- check cache up to date
   MTAPIRES          CheckUpToDate(const INT64 from,const INT64 to) const;
   //--- write cache
   virtual MTAPIRES  Write(INT64 from,INT64 to);
   //--- write missing keys to cache
   MTAPIRES          WriteMissing(const INT64 cache_from,const INT64 cache_to,INT64 &from,INT64 &to);
   //--- write cache range
   virtual MTAPIRES  WriteRange(const INT64 from,const INT64 to);
   //--- write cache range with limited request
   MTAPIRES          WriteRangeLimit(IMTDataset &data,const INT64 from,const INT64 to);
   //--- request data from base
   virtual MTAPIRES  SelectData(IMTDataset &data,INT64 from,INT64 to,UINT64 id_from)=0;
   //--- write data to cache
   virtual MTAPIRES  WriteData(const IMTDataset &data,UINT64 &id_last)=0;
   //--- logging
   virtual MTAPIRES  LogSelectError(MTAPIRES res,INT64 from,INT64 to,UINT64 id_from)=0;
   virtual MTAPIRES  LogWriteError(MTAPIRES res,INT64 from,INT64 to)=0;
   virtual MTAPIRES  LogWriteOk(INT64 from,INT64 to,UINT64 total,UINT time_select,UINT time_write)=0;
  };
//+------------------------------------------------------------------+
