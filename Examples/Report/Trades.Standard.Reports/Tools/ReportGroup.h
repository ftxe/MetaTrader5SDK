//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "TradeReport.h"
//+------------------------------------------------------------------+
//| Report with group parameter base class                           |
//+------------------------------------------------------------------+
class CReportGroup : public CTradeReport
  {
protected:
   //--- API interfaces
   IMTReportCacheKeySet *m_report_logins;    // report request logins
   IMTReportCacheKeySet *m_user_logins;      // user logins
   bool              m_user_valid;           // current user record valid

public:
   //--- constructor/destructor
                     CReportGroup(const MTReportInfo &info);
   virtual          ~CReportGroup(void);

protected:
   //--- base overrides
   virtual void      Clear(void) override;
   virtual MTAPIRES  Prepare(void) override;
   //--- Read user from dataset
   MTAPIRES          ReadUser(IMTDataset &dataset,UINT64 login,void *data,UINT size);
   //--- read from dataset
   static MTAPIRES   ReadLogins(IMTReportCacheKeySet &logins,IMTDataset &dataset,UINT64 &login_last);
  };
//+------------------------------------------------------------------+

