//+------------------------------------------------------------------+
//|                               MetaTrader 5 Daily.Standard.Report |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//---
#include "..\Tools\LogFileReader.h"
//---
#define DEFAULT_PROFIT_TRESHOLD    L"10.0"   // in default currency
#define DEFAULT_TICK_PIPS_TRESHOLD L"10"     // in pips
#define DEFAULT_MAX_LINE           L"50"
//--- 
#define AUTOMANAGER_LOGIN          0
#define AUTOMANAGER_NAME           L"Auto Execution"
//---
#define UNKNOWN_NAME               L"Unknown"
//+------------------------------------------------------------------+
//| Daily Dealing Report                                             |
//+------------------------------------------------------------------+
class CDailyDealingReport : public IMTReportContext
  {
private:
   //--- constants
   enum constants
     {
      MAX_MANAGERS=10,
     };
   //--- manager
   struct Manager
     {
      UINT64            login;                  // manager
      wchar_t           name[128];              // name
      UINT64            connections_count;      // connections counter
      UINT64            requests_count;         // requests counter
      UINT64            rejects_count;          // rejects counter
      UINT64            requotes_count;         // requotes counter
      double            profit_currency;        // managers`s profit in currency
      INT64             profit_pips;            // managers`s profit in pips
     };
   //--- suspect action of dealer
   struct SuspectAction
     {
      UINT64            login;                  // manager
      double            profit_currency;        // managers`s profit in currency
      INT64             profit_pips;            // managers`s profit in pips
      wchar_t           details[1024];          // log message
     };
   //--- ticks from dealer
   struct DealerTick
     {
      UINT64            login;                  // manager
      wchar_t           symbol[32];             // symbol
      UINT64            bid;                    // bid deviation
      UINT64            ask;                    // ask deviation
     };
   //--- arrays
   typedef TMTArray<Manager,64> ManagersArray;
   typedef TMTArray<SuspectAction,256> SuspectActionsArray;
   typedef TMTArray<DealerTick,256> TicksArray;
   //---
   IMTReportAPI*     m_api;                     // api interface
   //--- 
   SuspectActionsArray m_suspect_actions;       // suspect operations
   ManagersArray     m_managers;                // managers
   TicksArray        m_ticks;                   // ticks
   //--- param configs
   wchar_t           m_currency[32];            // currency for converting
   wchar_t           m_groups[128];             // groups
   UINT64            m_pips;                    // threshold pips from dealer`s ticks
   double            m_profit;                  // threshold dealer`s profit
   UINT64            m_max_line;                // max line for dealer action table
   //--- helper configs
   IMTUser*          m_user;                    // user
   IMTConSymbol*     m_symbol;                  // symbol
   IMTConGroup*      m_group;                   // group
   IMTConGateway*    m_gateway;                 // gateway
   UINT              m_digits;                  // currency digits
   //--- charts
   IMTReportChart*   m_managers_chart;          // managers chart
   //---
   CLogFileReader    m_file_reader;             // log file reader
   //--- static data
   static MTReportInfo s_info;                  // report information
   static ReportColumn s_columns_manager[];     // column descriptions total
   static ReportColumn s_columns_action[];      // column description suspect action
   static ReportColumn s_columns_tick[];        // column description dealer tick

public:
   //--- constructor/destructor
                     CDailyDealingReport(void);
   virtual          ~CDailyDealingReport(void);
   //--- get information about report
   static void       Info(MTReportInfo& info);
   //--- release plug-in
   virtual void      Release(void);
   //--- report generation
   virtual MTAPIRES  Generate(const UINT type,IMTReportAPI *api);

private:
   //--- create/release interfaces
   void              Clear(void);
   bool              CreateInterfaces(void);
   //--- get parameters
   MTAPIRES          GetParameters(void);
   //--- processing
   MTAPIRES          LoadInfo(void);
   bool              ParseUserManager(UINT64& user,UINT64& manager,const CMTStr& message,bool request=false);
   bool              ParseUser(UINT64& user,const CMTStr& message);
   bool              ParseActivateOrder(const CMTStr& message,double& profit,double& pips,CMTStr& symbol);
   bool              ParseManager(UINT64& manager,const CMTStr& message);
   bool              ParseTick(const CMTStr& message,CMTStr& symbol,double& bid,double& ask,int pos);
   bool              ParseDeletedPosition(const CMTStr& message,double& profit,double& pips,CMTStr& symbol,int pos);
   bool              ParseModifyPosition(const CMTStr& message,double& profit,double& pips,CMTStr& symbol,int pos);
   bool              ParseMessage(const CMTStr& message,double& profit,double& pips,CMTStr& symbol,int pos,bool confirm=true);
   bool              ParseMessageAutomate(const CMTStr& message,double& profit,double& pips,CMTStr& symbol,int pos);
   //--- get manager name
   void              GetManagerName(UINT64 login,CMTStr& name);
   //--- get file name
   bool              GetFileName(CMTStr& name,const INT64 day);
   //--- prepare charts
   MTAPIRES          PrepareManagersChart(void);
   //--- write html
   MTAPIRES          WriteAll(void);
   bool              WriteManagers(MTAPISTR& tag,MTAPIRES& retcode,UINT& counter);
   bool              WriteSuspectActions(MTAPISTR& tag,MTAPIRES& retcode,UINT& counter);
   bool              WriteSuspectTicks(MTAPISTR& tag,MTAPIRES& retcode,UINT& counter);
   //--- generate dashboard report
   MTAPIRES          GenerateDashboard(void);
   //--- generate dashboard report
   MTAPIRES          GenerateDashboardManager(void);
   //--- generate dashboard suspect action of dealer
   MTAPIRES          GenerateDashboardAction(void);
   //--- generate dashboard ticks from dealer
   MTAPIRES          GenerateDashboardTick(void);
   //--- add managers chart
   MTAPIRES          AddManagersChart(IMTDataset *data,UINT column,UINT left);
   //--- sorting
   static int        SortManagerByLogin(const void *left,const void *right);
   static int        SortManagerByRequests(const void *left,const void *right);
   static int        SearchManagerByLogin(const void *left,const void *right);
   static int        SortActionByProfit(const void *left,const void *right);
  };
//+------------------------------------------------------------------+

