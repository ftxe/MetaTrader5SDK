//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "ReportCache.h"
#include "..\Tools\ReportTop.h"
#include "..\Tools\ReportDictionary.h"
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CReportCache::CReportCacheContext::CReportCacheContext(IMTReportAPI &api,LPCWSTR name,const UINT version,const INT64 key_time_to_live) :
   m_res(MT_RET_ERR_PARAMS),m_api(api),m_cache(nullptr),m_value(nullptr)
  {
//--- check cache name
   if(!name || !*name)
      return;
//--- create report cache
   if(!(m_cache=api.ReportCacheCreate()))
     {
      m_res=MT_RET_ERR_MEM;
      return;
     }
//--- check key TTL exists
   if(key_time_to_live)
     {
      //--- get report cache with temporary keys
      if((m_res=api.ReportCacheGetTemporary(name,version,key_time_to_live,m_cache))!=MT_RET_OK)
         return;
     }
   else
     {
      //--- get report cache
      if((m_res=api.ReportCacheGet(name,version,m_cache))!=MT_RET_OK)
         return;
     }
//--- create report cache value
   if(!(m_value=m_cache->ValueCreate()))
      m_res=MT_RET_ERR_MEM;
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CReportCache::CReportCacheContext::~CReportCacheContext(void)
  {
//--- release interfaces
   if(m_cache)
     {
      m_cache->Release();
      m_cache=nullptr;
     }
   if(m_value)
     {
      m_value->Release();
      m_value=nullptr;
     }
  }
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CReportCache::CReportCache(CReportCacheContext &context) :
   m_api(context.m_api),m_cache(context.Detach(context.m_cache)),m_value(context.Detach(context.m_value))
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CReportCache::~CReportCache(void)
  {
//--- release interfaces
   m_value.Release();
   m_cache.Release();
  }
//+------------------------------------------------------------------+
//| release object                                                   |
//+------------------------------------------------------------------+
void CReportCache::Release(void)
  {
   delete this;
  }
//+------------------------------------------------------------------+
//| read data by key                                                 |
//+------------------------------------------------------------------+
MTAPIRES CReportCache::ReadData(const UINT64 key,const void *&data,const UINT data_size) const
  {
//--- read cache value by key
   MTAPIRES res=m_cache.ReadValue(key,&m_value);
   if(res!=MT_RET_OK)
      return(LogError(res,L"read cache value by key %I64u",key));
//--- get data from cache value
   if((res=DataFromValue(data,data_size))!=MT_RET_OK)
      return(LogError(res,L"get data from cache value by key %I64u",key));
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| read data from next value                                        |
//+------------------------------------------------------------------+
MTAPIRES CReportCache::ReadDataFirst(const IMTReportCacheKeySet &keys,UINT64 &key,const void *&data,const UINT data_size) const
  {
//--- read cache values by keys array
   MTAPIRES res=m_cache.ReadValues(&keys,&m_value);
   if(res!=MT_RET_OK)
      return(LogError(res,L"read cache values by keys array first"));
//--- get data from cache value
   if((res=DataFromValue(data,data_size))!=MT_RET_OK)
      return(LogError(res,L"get data from cache value first"));
//--- get key from cache value
   key=m_value.Key();
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| read data from next value                                        |
//+------------------------------------------------------------------+
MTAPIRES CReportCache::ReadDataNext(UINT64 &key,const void *&data,const UINT data_size) const
  {
//--- iterate to next value
   MTAPIRES res=m_value.Next();
   if(res!=MT_RET_OK)
      return(LogError(res,L"read cache values by keys array next"));
//--- get data from cache value
   if((res=DataFromValue(data,data_size))!=MT_RET_OK)
      return(LogError(res,L"get data from cache value next"));
//--- get key from cache value
   key=m_value.Key();
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| get data from cache value                                        |
//+------------------------------------------------------------------+
MTAPIRES CReportCache::DataFromValue(const void *&data,const UINT data_size) const
  {
//--- get value size
   const UINT size=m_value.Size();
//--- check empty value
   if(!size)
     {
      data=nullptr;
      return(MT_RET_OK);
     }
//--- check value size
   if(size<data_size)
      return(LogError(MT_RET_ERROR,L"check value size"));
//--- retrive data
   data=m_value.Data();
   if(!data)
      return(LogError(MT_RET_ERROR,L"retrive data"));
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Read top names                                                   |
//+------------------------------------------------------------------+
MTAPIRES CReportCache::ReadTopNames(const UINT dictionary_id,const CReportTopBase &top,CReportDictionary &names) const
  {
//--- top count
   const UINT total=top.Total();
   if(!total)
      return(MT_RET_OK);
//--- initialize top names
   if(!names.Initialize(total))
      return(LogError(MT_RET_ERR_MEM,L"initialize top names"));
//--- initialize top names
   for(UINT i=0;i<total;i++)
     {
      //--- get top pos
      const UINT *pos=top.Pos(i);
      if(!pos)
         return(LogError(MT_RET_ERROR,L"get top pos"));
      //--- check other
      if(*pos==CReportTopBase::TOP_POS_OTHER)
        {
         //--- add other top name
         if(!names.String(i,L"Other"))
            return(LogError(MT_RET_ERR_MEM,L"add other top name"));
        }
      else
        {
         //--- get top name
         LPCWSTR name=nullptr;
         const MTAPIRES res=ReadDictionaryString(dictionary_id,*pos,name);
         if(res!=MT_RET_OK)
            return(res);
         //--- store top name
         if(!names.String(i,name && *name ? name : L"None"))
            return(LogError(MT_RET_ERR_MEM,L"store top name"));
        }
     }
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| report generation time ceil                                      |
//+------------------------------------------------------------------+
INT64 CReportCache::Now(IMTReportAPI &api,CReportParameter &params)
  {
//--- report generation time
   const INT64 ctm=api.TimeGeneration();
   if(!ctm)
      return(0);
//--- get maximum data latency parameter
   INT64 latency=0;
   if(params.ValueInt(latency,PARAMETER_DATA_LATENCY_MAX,DEFAULT_DATA_LATENCY_MAX)!=MT_RET_OK)
      return(ctm);
//--- check for zero
   if(!latency)
      return(ctm);
//--- parse time
   tm ttm={};
   if(!SMTTime::ParseTime(ctm,&ttm))
      return(0);
//--- depending on latency
   if(latency>=SECONDS_IN_MINUTE)
     {
      //--- floor minutes and zero seconds
      const int latency_min=(int)latency/SECONDS_IN_MINUTE;
      ttm.tm_min=(ttm.tm_min/latency_min)*latency_min;
      ttm.tm_sec=0;
     }
   else
     {
      //--- floor seconds
      ttm.tm_sec=(ttm.tm_sec/(int)latency)*(int)latency;
     }
//--- make time
   return(SMTTime::MakeTime(&ttm));
  }
//+------------------------------------------------------------------+
