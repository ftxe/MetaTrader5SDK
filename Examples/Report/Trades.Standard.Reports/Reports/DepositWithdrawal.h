//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Deposit and Withdrawal Report                                    |
//+------------------------------------------------------------------+
class CDepositWithdrawal : public IMTReportContext
  {
private:
   //--- column ids
   enum columns
     {
      COLUMN_DEAL           =1,
      COLUMN_LOGIN          =2,
      COLUMN_NAME           =3,
      COLUMN_TIME           =4,
      COLUMN_COMMENT        =5,
      COLUMN_AMOUNT         =6,
      COLUMN_CURRENCY       =7,
      COLUMN_CURRENCY_DIGITS=8,
     };
   //--- table record
   #pragma pack(push,1)
   struct TableRecord
     {
      UINT64            deal;            // deal
      UINT64            login;           // login
      wchar_t           name[128];       // client name
      INT64             date;            // deal time
      wchar_t           comment[32];     // client name
      double            amount;          // amount
      wchar_t           currency[32];    // group currency
      UINT              currency_digits; // group currency digits
     };
   #pragma pack(pop)
   //---
   typedef TMTArray<TableRecord> TableRecordArray;

private:
   //---
   static MTReportInfo s_info;            // report information
   static ReportColumn s_columns[];       // column descriptions   
   //---
   IMTReportAPI     *m_api;               // api interface
   IMTDealArray     *m_deals;             // deal array interface
   IMTUser          *m_user;              // user interface
   IMTConGroup      *m_group;             // group config interface
   TableRecordArray  m_summaries;         // summaries

public:
   //--- constructor/destructor
                     CDepositWithdrawal(void);
   virtual          ~CDepositWithdrawal(void);
   //--- get information about report
   static void       Info(MTReportInfo& info);
   //--- release plug-in
   virtual void      Release(void);
   //--- report generation
   virtual MTAPIRES  Generate(const UINT type,IMTReportAPI *api);
private:
   //--- table support
   MTAPIRES          TablePrepare(void);
   MTAPIRES          TableWrite(const UINT64 login);
   MTAPIRES          TableWriteSummaries(void);
   //--- clear
   void              Clear(void);
   //--- sort functions
   static int        SortSummary(const void *left,const void *right);
  };
//+------------------------------------------------------------------+
