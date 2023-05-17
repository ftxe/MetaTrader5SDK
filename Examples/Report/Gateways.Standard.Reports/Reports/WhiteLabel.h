//+------------------------------------------------------------------+
//|                           MetaTrader 5 Gateways.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Gateways White Label Report                                      |
//+------------------------------------------------------------------+
class CWhiteLabel : public IMTReportContext
  {
private:
   //--- constants
   enum constants
     {
      REPORT_INDEX=2,
     };
   //--- account
   struct Agent
     {
      UINT64            login;               // agent
      wchar_t           symbol[32];          // symbol
      UINT              deals;               // amount of deals
      UINT64            lots;                // lots
      double            amount_raw;          // raw amount
      wchar_t           amount_currency[32]; // raw amount currency
      double            amount;              // amount
      double            amount_rate;         // rate of amount currency to our currency
     };
   //--- account array
   typedef TMTArray<Agent,1024> AgentArray;
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
   IMTConParam      *m_param;             // param
   IMTConGroup      *m_group;             // group
   IMTConSymbol     *m_symbol;            // symbol
   IMTDealArray     *m_deals;             // deals
   IMTUser          *m_user;              // user
   //--- report data
   wchar_t           m_currency[32];      // currency
   UINT              m_currency_digits;   // currency digits
   AgentArray        m_agents;            // agents

public:
   //--- constructor/destructor                    
                     CWhiteLabel(void);
   virtual          ~CWhiteLabel(void);
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
   //--- agent iterator
   bool              AgentGet(const UINT pos,UINT& index);
   //--- sorting/searching
   static int        SortBySymbol(const void *left,const void *right);
   static int        SortByAmount(const void *left,const void *right);
   static int        SortRates(const void *left,const void *right);
   static int        SearchRates(const void *left,const void *right);
  };
//+------------------------------------------------------------------+

