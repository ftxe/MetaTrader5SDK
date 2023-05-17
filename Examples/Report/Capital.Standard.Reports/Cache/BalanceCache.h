//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportCacheSelectTime.h"
#include "..\Tools\DatasetField.h"
//+------------------------------------------------------------------+
//| Balance cache class                                              |
//+------------------------------------------------------------------+
class CBalanceCache : public CReportCacheSelectTime
  {
public:
   //--- User balance structure
   //--- Update version constant below when change this structure!
   #pragma pack(push,1)
   struct UserBalance
     {
      UINT              currency_digits;        // currency digits
      UINT              aggregate_count;        // deal aggregates count
     };
   //--- Deal balance aggregate info structure
   struct DealInfo
     {
      unsigned          deposit : 1;            // deposit aggregate flag
      unsigned          day     : 31;           // aggregate day
     };
   //--- Deal balance aggregate structure
   struct DealBalance
     {
      union
        {
         DealInfo          info;                   // aggregate info
         UINT              key;                    // key
        };
      UINT              count;                  // deals count
      double            value;                  // balance value
     };
   #pragma pack(pop)
   //--- User balance class
   class CUserBalance
     {
   protected:
      const char       *m_data=nullptr;         // data pointer
      UINT              m_size=0;               // data size

   public:
      //--- clear value
      void              Clear(void);
      //--- set value
      MTAPIRES          Value(IMTReportCacheValue &value);
      //--- data access
      const UserBalance* User(void) const;
      const DealBalance* Deal(const UINT pos) const;
     };

private:
   //--- Cache version constant
   //--- Update it when changing structure above!
   static const UINT s_cache_version=4;
   //--- Deal record
   #pragma pack(push,1)
   struct DealRecord
     {
      UINT64            login;                  // login
      INT64             time;                   // time
      double            profit;                 // profit
      UINT              currency_digits;        // group currency digits
     };
   #pragma pack(pop)
   //--- User balance write class
   class CUserBalanceWrite : public CUserBalance
     {
   private:
      CMTMemPack        m_buffer;               // data write buffer
      UINT64            m_login=0;              // current balance user login
      //--- limits
      static const UINT s_aggregate_max=((1<<22)-sizeof(UserBalance))/sizeof(DealBalance);
      static const UINT s_buffer_max=s_aggregate_max *sizeof(DealBalance)+sizeof(UserBalance);

   public:
      //-- current balance user login
      UINT64            Login(void) const { return(m_login); }
      //--- write balance to cache
      MTAPIRES          WriteBalance(IMTReportCache &cache);
      //--- add balance with deal to cache writer
      MTAPIRES          AddBalanceDeal(const DealRecord &deal);

   private:
      //--- aggregate balance deal
      MTAPIRES          AggreateDeal(const DealInfo &info,const DealRecord &deal);
      //--- init aggregate by balance deal
      void              AggreateInit(DealBalance &balance,const DealInfo &info,const DealRecord &deal);
      //--- add balance deal to aggregate
      void              AggreateAdd(DealBalance &balance,const DealRecord &deal);
      //--- reallocate cache writer buffer to fit at least one deal
      bool              AllocateDeal(const UINT size);
      //--- search deal balance static method
      static int        SearchDealBalance(const void *left,const void *right);
      //--- sort deal balance static method
      static int        SortDealBalance(const void *left,const void *right);
      //--- calculate upper power of 2
      static UINT       UpperPowerOfTwo(UINT v);
     };

private:
   //--- static data
   static DatasetField s_fields[];              // deal request dataset fields descriptions

public:
   //--- create class object
   static CBalanceCache* Create(IMTReportAPI &api,MTAPIRES &res);
   //--- read user balance
   MTAPIRES          ReadBalance(const UINT64 login,CUserBalance &balance) const;

private:
   explicit          CBalanceCache(CReportCacheContext &context);
   virtual          ~CBalanceCache(void);
   //--- write deals to balances
   virtual MTAPIRES  WriteData(const IMTDataset &deals) override;
   //--- request deals from base
   virtual MTAPIRES  SelectData(IMTDataset &deals,const IMTReportCacheKeySet &keys,const INT64 from,const INT64 to) override;
   //--- fill deals request
   MTAPIRES          DealRequest(IMTDatasetRequest &request,const IMTReportCacheKeySet &logins,const INT64 from,const INT64 to);
  };
//+------------------------------------------------------------------+
