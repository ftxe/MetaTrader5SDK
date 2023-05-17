//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "MoneyFlowDailySectionReport.h"
//+------------------------------------------------------------------+
//| Money Flow Daily by Country report                               |
//+------------------------------------------------------------------+
class CMoneyFlowDailyCountry : public CMoneyFlowDailySection
  {
private:
   static MTReportInfo s_info;                        // static report info

public:
                     CMoneyFlowDailyCountry(void);
   virtual          ~CMoneyFlowDailyCountry(void);
   //--- report information
   static void       Info(MTReportInfo &info);
   //--- fabric method
   static IMTReportContext* Create(void);
  };
//+------------------------------------------------------------------+
