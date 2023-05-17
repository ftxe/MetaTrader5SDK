//+------------------------------------------------------------------+
//|                           MetaTrader 5 Gateways.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Gateways Profit Report                                           |
//+------------------------------------------------------------------+
class CProfit : public IMTReportContext
  {
private:
   //--- constants
   enum constants
     {
      REPORT_INDEX=1,
     };
   //--- profit
   struct Profit
     {
      wchar_t           gateway[16];         // gateway
      UINT64            gateway_id;          // gateway id
      wchar_t           symbol[32];          // symbol
      UINT              deals;               // amount of deals
      UINT64            lots;                // lots
      double            amount_raw;          // raw amount
      wchar_t           amount_currency[32]; // raw amount currency
      double            amount;              // amount
      double            amount_rate;         // rate of amount currency to our currency
      INT64             profit_pips;         // profit in pips
      double            profit_raw;          // raw profit
      wchar_t           profit_currency[32]; // raw profit currency
      double            profit;              // profit
      double            profit_rate;         // rate of profit currency to our currency
     };
   //--- profit array
   typedef TMTArray<Profit,1024> ProfitArray;
   //--- rate info
   struct RateInfo
     {
      wchar_t           currency[32];        // base currency
      double            rate_buy;            // rate sell
      double            rate_sell;           // rate buy
     };
   //--- rate array
   typedef TMTArray<RateInfo,256> RateInfoArray;

private:
   static MTReportInfo s_info;            // report information
   //--- api data
   IMTReportAPI     *m_api;               // api interface
   IMTConReport     *m_report;            // report
   IMTConParam      *m_param;             // report param
   IMTConGroup      *m_group;             // group
   IMTConSymbol     *m_symbol;            // symbol
   IMTDealArray     *m_deals;             // deals
   //--- report data
   wchar_t           m_currency[32];      // currency
   UINT              m_currency_digits;   // currency digits
   ProfitArray       m_profits;           // profits

public:
   //--- constructor/destructor
                     CProfit(void);
   virtual          ~CProfit(void);
   //--- get information about report
   static UINT       Index(void) { return(REPORT_INDEX); }
   static void       Info(MTReportInfo& info);
   //--- release plug-in
   virtual void      Release(void);
   //--- report generation
   virtual MTAPIRES  Generate(const UINT type,IMTReportAPI *api);

private:
   //--- clear
   void              Clear(void);
   //--- collecting data
   MTAPIRES          GenerateCollect(void);
   //--- collecting data by user
   MTAPIRES          GenerateUser(const UINT64 login);
   //--- generating HTML
   MTAPIRES          GenerateHtml(void);
   //--- gateway iterator
   bool              GatewayGet(const UINT pos,UINT& index);
   //--- sorting/searching
   static int        SortBySymbol(const void *left,const void *right);
   static int        SortByProfit(const void *left,const void *right);
   static int        SortRates(const void *left,const void *right);
   static int        SearchRates(const void *left,const void *right);
  };
//+------------------------------------------------------------------+

