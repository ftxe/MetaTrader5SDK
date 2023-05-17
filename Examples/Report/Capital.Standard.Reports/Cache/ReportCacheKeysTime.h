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
class CReportCacheKeysTime : public CReportCache
  {
protected:
   //--- cache context class
   class CReportCacheKeysTimeContext : public CReportCacheContext
     {
   public:
      IMTDatasetRequest *m_request;       // dataset request interface pointer

   public:
      explicit          CReportCacheKeysTimeContext(IMTReportAPI &api,LPCWSTR name,UINT version);
      virtual          ~CReportCacheKeysTimeContext(void);
     };

private:
   IMTDatasetRequest &m_request;          // dataset request interface reference
   UINT              m_keys_total;        // request keys total count for logging

public:
   //--- update cache
   MTAPIRES          Update(const IMTReportCacheKeySet &keys,INT64 from,INT64 to);

protected:
   explicit          CReportCacheKeysTime(CReportCacheKeysTimeContext &context);
   virtual          ~CReportCacheKeysTime(void);
   //--- check cache up to date
   MTAPIRES          CheckUpToDate(const IMTReportCacheKeySet &keys,INT64 from,INT64 to) const;
   //--- write cache
   virtual MTAPIRES  Write(const IMTReportCacheKeySet &keys,INT64 from,INT64 to);
   //--- write missing keys to cache
   MTAPIRES          WriteMissing(const IMTReportCacheKeySet &keys,const IMTReportCacheKeySet &keys_missing,INT64 cache_from,INT64 cache_to,INT64 &from,INT64 &to);
   //--- write cache range
   virtual MTAPIRES  WriteRange(IMTDatasetRequest &request,INT64 from,INT64 to);
   //--- write cache range with limited request
   MTAPIRES          WriteRangeLimit(IMTDataset &data,IMTDatasetRequest &request,INT64 from,INT64 to);
   //--- prepare request for all keys
   MTAPIRES          PrepareRequestAll(const IMTReportCacheKeySet &keys,IMTReportCacheKeySet &keys_all);
   //--- prepare request for key set
   MTAPIRES          PrepareRequestKeys(const IMTReportCacheKeySet &keys);
   //--- prepare request
   virtual MTAPIRES  PrepareRequest(IMTDatasetRequest &request,const IMTReportCacheKeySet &keys)=0;
   //--- request data from base
   virtual MTAPIRES  SelectData(IMTDataset &data,IMTDatasetRequest &request,INT64 from,INT64 to,UINT64 id_from)=0;
   //--- write data to cache
   virtual MTAPIRES  WriteData(const IMTDataset &data,UINT64 &id_last)=0;
   //--- write missing keys
   virtual MTAPIRES  WriteMissingKeys(const IMTReportCacheKeySet &keys) { return(keys.Total() ? m_cache.WriteMissingKeys(&keys) : MT_RET_OK); }
   //--- logging
   virtual MTAPIRES  LogSelectError(MTAPIRES res,UINT keys,INT64 from,INT64 to,UINT64 id_from)=0;
   virtual MTAPIRES  LogWriteError(MTAPIRES res,INT64 from,INT64 to)=0;
   virtual MTAPIRES  LogWriteOk(INT64 from,INT64 to,UINT64 total,UINT keys,UINT time_select,UINT time_write)=0;
  };
//+------------------------------------------------------------------+
