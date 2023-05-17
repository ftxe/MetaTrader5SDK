//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportCacheTime.h"
#include "UserCache.h"
#include "..\Tools\DatasetField.h"
//+------------------------------------------------------------------+
//| Deal cache class                                                 |
//+------------------------------------------------------------------+
class CDealCache : public CReportCacheTime
  {
public:
   //--- Deal key structure
   //--- Update version constant below when change this structure!
   #pragma pack(push,1)
   struct DealKey
     {
      unsigned          hour     : 24;
      unsigned          currency : 10;
      unsigned          country  : 12;
      unsigned          reason   :  6;
      unsigned          reserved : 12;
     };
   //--- Deal hour structure
   struct DealHour
     {
      UINT              profit_count;           // profit deals count
      UINT              loss_count;             // loss deals count
      double            profit_volume;          // profit volume (in currency)
      double            loss_volume;            // loss volume (in currency)
      double            profit;                 // profit
      double            loss;                   // loss
     };
   #pragma pack(pop)
   //--- Deal key structure fields ids
   enum EnDealKeyFields
     {
      DEAL_KEY_FIELD_CURRENCY=0,
      DEAL_KEY_FIELD_COUNTRY =1,
      DEAL_KEY_FIELD_REASON  =2,
     };

private:
   //--- Cache version constant
   //--- Update it when changing structure above!
   static const UINT s_cache_version=2;
   //--- Deal record
   #pragma pack(push,1)
   struct DealRecord
     {
      UINT64            login;                  // login
      INT64             time;                   // time
      UINT64            volume;                 // volume
      double            contract_size;          // symbol contract size
      double            rate_profit;            // profit conversion rate
      double            profit;                 // profit
      UINT              reason;                 // reason
      UINT              currency_digits;        // group currency digits
     };
   #pragma pack(pop)
   //--- Deal hour write class
   class CDealHourWrite
     {
   private:
      IMTReportCache   &m_cache;                // cache interface
      IMTReportCacheValue &m_value;             // report cache value interface reference
      UINT64            m_key;                  // deal key
      DealHour          m_hour;                 // deal hour

   public:
      CDealHourWrite(IMTReportCache &cache,IMTReportCacheValue &value) :
                        m_cache(cache),m_value(value),m_key(0),m_hour{0} {}
      //-- current deal hour key
      UINT64            Key(void) const { return(m_key); }
      //--- read deal hour from cache
      MTAPIRES          Read(const UINT64 key);
      //--- write deal hour to cache
      MTAPIRES          Write(void);
      //--- add deal to cache writer
      MTAPIRES          AddDeal(const DealRecord &deal);
     };

private:
   //--- cache context class
   class CDealCacheContext : public CReportCacheContext
     {
   public:
      IMTReportCacheKeySet *m_keys;             // keys set

   public:
                        CDealCacheContext(IMTReportAPI &api,const CMTStr &name);
      virtual          ~CDealCacheContext(void);
     };

private:
   CUserCache       &m_users;                   // users cache
   const IMTReportCacheKeySet &m_logins;        // logins set
   IMTReportCacheKeySet &m_keys;                // keys set
   //--- static data
   static DatasetField s_fields[];              // deal request dataset fields descriptions
   static UINT64     s_actions[];               // deal request actions
   static UINT64     s_entries[];               // deal request entries

public:
   //--- create class object
   static CDealCache* Create(IMTReportAPI &api,CUserCache &users,CReportParameter &params,CCurrencyConverterCache &currency,MTAPIRES &res);
   //--- read currency string
   MTAPIRES          ReadCurrency(const DealKey &key,LPCWSTR &currency) const { return(ReadDictionaryString(DEAL_KEY_FIELD_CURRENCY,key.currency,currency)); }
   //--- get deal hour key
   static const DealKey& Key(const UINT64 &key) { return(*(const DealKey*)(&key)); }
   //--- get deal hour key field value
   static UINT       KeyField(const DealKey &key,const EnDealKeyFields field) { return(field==DEAL_KEY_FIELD_COUNTRY ? key.country : key.reason); }

private:
                     CDealCache(CDealCacheContext &context,CUserCache &users);
   virtual          ~CDealCache(void)=default;
   //--- request deals from base
   virtual MTAPIRES  SelectData(IMTDataset &deals,INT64 from,INT64 to,UINT64 id_from) override;
   //--- write deals to balances
   virtual MTAPIRES  WriteData(const IMTDataset &deals,UINT64 &id_last) override;
   //--- fill deals request
   MTAPIRES          DealRequest(IMTDatasetRequest &request,const INT64 from,const INT64 to);
   //--- write deals to deals hours
   MTAPIRES          WriteDeals(const IMTDataset &deals);
   //--- make deal key
   static UINT64     MakeKey(const DealRecord &deal,const UINT currency,const UINT country);
   //--- calculate keys set hash
   static UINT64     KeyHash(const IMTReportCacheKeySet &keys);
   //--- compression function for Merkle-Damgard construction
   static UINT64     HashMix(UINT64 h);
   //--- logging
   virtual MTAPIRES  LogSelectError(const MTAPIRES res,const INT64 from,const INT64 to,const UINT64 id_from) override
     {
      return(LogErrorTime(res,L"Deals selection with creation time",from,to));
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
