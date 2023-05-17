//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Agents Report                                                    |
//+------------------------------------------------------------------+
class CAgents : public IMTReportContext
  {
private:
   //--- column ids
   enum columns
     {
      COLUMN_DEAL           =1,
      COLUMN_LOGIN          =2,
      COLUMN_NAME           =3,
      COLUMN_TIME           =4,
      COLUMN_TYPE           =5,
      COLUMN_COMMENT        =6,
      COLUMN_PROFIT         =7,
      COLUMN_CURRENCY       =8,
      COLUMN_CURRENCY_DIGITS=9,
     };
   //--- table record
   #pragma pack(push,1)
   struct TableRecord
     {
      UINT64            deal;             // deal
      UINT64            login;            // login
      wchar_t           name[128];        // client name
      INT64             time_create;      // deal time
      wchar_t           comment[32];      // comment
      double            profit;           // profit
      wchar_t           currency[32];     // group currency
      UINT              type;             // type
      UINT              currency_digits;  // group currency digits
     };
   #pragma pack(pop)
   //--- array type
   typedef TMTArray<TableRecord> TableRecordArray;

private:
   //---
   static MTReportInfo s_info;            // report information     
   static ReportColumn s_columns[];       // column descriptions
   //--- 
   IMTReportAPI     *m_api;               // api
   IMTDealArray     *m_deals;             // deal array
   IMTUser          *m_user;              // user
   IMTConGroup      *m_group;             // group
   TableRecordArray  m_summaries;         // summaries

public:
   //--- constructor/destructor
                     CAgents(void);
   virtual          ~CAgents(void);
   //--- get information about report
   static void       Info(MTReportInfo& info);
   //--- release plug-in
   virtual void      Release(void);
   //--- report generation
   virtual MTAPIRES  Generate(const UINT type,IMTReportAPI *api);

private:
   //--- clear
   void              Clear(void);
   //--- table management
   MTAPIRES          TablePrepare(void);
   MTAPIRES          TableWrite(const UINT64 login);
   MTAPIRES          TableWriteSummaries(void);
   //--- sort functions
   static int        SortSummary(const void *left,const void *right);
  };
//+------------------------------------------------------------------+

