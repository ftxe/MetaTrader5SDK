//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportCacheSelectTime.h"
#include "..\Tools\DatasetField.h"
//+------------------------------------------------------------------+
//| Deposit cache class                                              |
//+------------------------------------------------------------------+
class CDepositCache : public CReportCacheSelectTime
  {
public:
   //--- User deposit structure
   //--- Update version constant below when change this structure!
   #pragma pack(push,1)
   struct UserDeposit
     {
      double            amount;                 // deposit amount
      UINT              count;                  // deposit count
      INT64             first_time;             // registration time
      double            first;                  // FTD
      UINT              currency_digits;        // currency digits
      //--- initialize
      void              Init(const double value,const INT64 ctm,const UINT _currency_digits);
      //--- deposit addition
      void              DepositAdd(const double value,const INT64 ctm);
     };
   #pragma pack(pop)

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
      double            profit;                 // profit
      UINT              currency_digits;        // group currency digits
     };
   #pragma pack(pop)

private:
   //--- static data
   static DatasetField s_fields[];              // deal request dataset fields descriptions

public:
   //--- create class object
   static CDepositCache* Create(IMTReportAPI &api,MTAPIRES &res);
   //--- read user deposit
   MTAPIRES          ReadDeposit(const UINT64 login,const UserDeposit *&deposit) const { return(CReportCache::ReadData(login,(const void*&)deposit,sizeof(*deposit))); }

private:
   explicit          CDepositCache(CReportCacheContext &context);
   virtual          ~CDepositCache(void);
   //--- write deals to deposits
   virtual MTAPIRES  WriteData(const IMTDataset &deals) override;
   //--- request deals from base
   virtual MTAPIRES  SelectData(IMTDataset &deals,const IMTReportCacheKeySet &keys,const INT64 from,const INT64 to) override;
   //--- fill deals request
   MTAPIRES          DealRequest(IMTDatasetRequest &request,const IMTReportCacheKeySet &logins,const INT64 from,const INT64 to);
  };
//+------------------------------------------------------------------+
