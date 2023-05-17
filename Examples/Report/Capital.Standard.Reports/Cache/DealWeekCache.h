//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportCacheTime.h"
#include "UserGroupCache.h"
#include "..\Tools\DatasetField.h"
//+------------------------------------------------------------------+
//| Deal week aggregates cache class                                 |
//+------------------------------------------------------------------+
class CDealWeekCache : public CReportCacheTime
  {
public:
   //--- Deal key structure
   //--- Update version constant below when change this structure!
   #pragma pack(push,1)
   struct DealKey
     {
      unsigned          week     : 24;
      unsigned          currency : 10;
      unsigned          group    : 20;
      unsigned          reserved : 10;
     };
   //--- Deal aggregate structure
   struct Deal
     {
      UINT              count;                  // deals count
      double            volume;                 // volume (in currency)
      double            value;                  // value of profit or loss (in currency)
     };
   //--- Balance aggregate structure
   struct Balance
     {
      UINT              count;                  // deals count
      double            value;                  // value (in currency)
     };
   //--- Deal week aggregate structure
   struct DealWeek
     {
      Deal              profit;                 // profit deals
      Deal              loss;                   // loss deals
      Balance           deposit;                // deposits
      Balance           withdrawal;             // withdrawals
     };
   #pragma pack(pop)
   //--- Deal key structure fields ids
   enum EnDealKeyFields
     {
      DEAL_KEY_FIELD_CURRENCY=0,
      DEAL_KEY_FIELD_GROUP   =1,
     };

private:
   //--- Cache version constant
   //--- Update it when changing structure above!
   static const UINT s_cache_version=1;
   //--- Deal record
   #pragma pack(push,1)
   struct DealRecord
     {
      UINT64            login;                  // login
      INT64             time;                   // time
      UINT              action;                 // action
      UINT              entry;                  // entry
      UINT64            volume;                 // volume
      double            contract_size;          // symbol contract size
      double            rate_profit;            // profit conversion rate
      double            profit;                 // profit
      UINT              currency_digits;        // group currency digits
     };
   #pragma pack(pop)
   //--- Deal week write class
   class CDealWeekWrite
     {
   private:
      IMTReportCache   &m_cache;                // cache interface
      IMTReportCacheValue &m_value;             // report cache value interface reference
      UINT64            m_key;                  // deal key
      DealWeek          m_week;                 // deal week

   public:
      CDealWeekWrite(IMTReportCache &cache,IMTReportCacheValue &value) :
                        m_cache(cache),m_value(value),m_key(0),m_week{} {}
      //-- current deal week key
      UINT64            Key(void) const { return(m_key); }
      //--- read deal week from cache
      MTAPIRES          Read(const UINT64 key);
      //--- write deal week to cache
      MTAPIRES          Write(void);
      //--- add deal to cache writer
      MTAPIRES          AddDeal(const DealRecord &deal);

   private:
      //--- add balance deal to cache writer
      MTAPIRES          AddBalanceDeal(const DealRecord &deal);
     };

private:
   CUserGroupCache  &m_users;                   // users cache
   //--- static data
   static const DatasetField s_fields[];        // deal request dataset fields descriptions
   static const UINT64 s_actions[];             // deal request actions
   static const UINT64 s_entries[];             // deal request entries
   static const UINT s_request_limit=16384;     // deals selection request limit

public:
   //--- create class object
   static CDealWeekCache* Create(IMTReportAPI &api,CUserGroupCache &users,CReportParameter &params,CCurrencyConverterCache &currency,MTAPIRES &res);
   //--- update deal week aggregates
   MTAPIRES          Update(INT64 from,INT64 to);
   //--- read currency string
   MTAPIRES          ReadCurrency(const DealKey &key,LPCWSTR &currency) const { return(ReadDictionaryString(DEAL_KEY_FIELD_CURRENCY,key.currency,currency)); }
   //--- get deal week key
   static const DealKey& Key(const UINT64 &key) { return(*(const DealKey*)(&key)); }

private:
                     CDealWeekCache(CReportCacheContext &context,CUserGroupCache &users);
   virtual          ~CDealWeekCache(void)=default;
   //--- write cache
   virtual MTAPIRES  Write(INT64 from,INT64 to) override;
   //--- write cache range
   virtual MTAPIRES  WriteRange(INT64 from,INT64 to) override;
   //--- request deals from base
   virtual MTAPIRES  SelectData(IMTDataset &deals,INT64 from,INT64 to,UINT64 id_from) override;
   //--- fill deals request
   MTAPIRES          DealRequest(IMTDatasetRequest &request,INT64 from,INT64 to,UINT64 id_from);
   //--- write deals to deals aggregates
   virtual MTAPIRES  WriteData(const IMTDataset &deals,UINT64 &id_last) override;
   //--- make deal key
   static UINT64     MakeKey(const DealRecord &deal,UINT currency,UINT group);
   //--- end of month
   static INT64      MonthEnd(INT64 ctm,INT64 to);
   //--- logging
   virtual MTAPIRES  LogSelectError(const MTAPIRES res,const INT64 from,const INT64 to,const UINT64 id_from) override
     {
      return(LogErrorTime(res,L"Deals selection from #%I64u with creation time",from,to,id_from));
     }
   virtual MTAPIRES  LogWriteError(const MTAPIRES res,const INT64 from,const INT64 to) override
     {
      return(LogErrorTime(res,L"Deals aggregation with creation time",from,to));
     }
   virtual MTAPIRES  LogWriteOk(const INT64 from,const INT64 to,const UINT64 total,const UINT time_select,const UINT time_write) override
     {
      return(LogOkTime(L"Selected %I64u deals in %u ms, aggregated in %u ms, creation time",from,to,total,time_select,time_write));
     }
  };
//+------------------------------------------------------------------+
