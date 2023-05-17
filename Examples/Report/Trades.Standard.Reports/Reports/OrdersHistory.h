//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Orders History Report                                            |
//+------------------------------------------------------------------+
class COrdersHistory : public IMTReportContext
  {
private:
   //--- column ids
   enum columns
     {
      COLUMN_ORDER          =1,
      COLUMN_ID             =2,
      COLUMN_LOGIN          =3,
      COLUMN_NAME           =4,
      COLUMN_SETUP_TIME     =5,
      COLUMN_TYPE           =6,
      COLUMN_SYMBOL         =7,
      COLUMN_VOLUME         =8,
      COLUMN_SETUP_PRICE    =9,
      COLUMN_SL             =10,
      COLUMN_TP             =11,
      COLUMN_DONE_TIME      =12,
      COLUMN_DONE_PRICE     =13,
      COLUMN_REASON         =14,
      COLUMN_STATE          =15,
      COLUMN_COMMENT        =16,
      COLUMN_DIGITS         =17,
     };
   //--- table record
   #pragma pack(push,1)
   struct TableRecord
     {
      UINT64            order;         // order
      wchar_t           id[32];        // external id
      UINT64            login;         // login
      wchar_t           name[128];     // client name
      INT64             time_setup;    // order setup time
      UINT              type;          // order type
      wchar_t           symbol[32];    // symbol
      UINT64            volume[2];     // initial / current volume
      double            price_setup;   // price
      double            sl;            // sl
      double            tp;            // tp
      INT64             time_done;     // time done
      double            price_done;    // price
      UINT              reason;        // reason
      UINT              state;         // reason
      wchar_t           comment[32];   // comment
      UINT              digits;        // symbol digits 
     };
   #pragma pack(pop)
   //---
   typedef TMTArray<TableRecord> TableRecordArray;

private:
   //---
   static MTReportInfo s_info;         // report information
   static ReportColumn s_columns[];    // column descriptions   
   //---
   IMTReportAPI     *m_api;            // api
   IMTOrderArray    *m_orders;         // order array interface
   IMTUser          *m_user;           // user interface
   IMTConGroup      *m_group;          // group config interface
   TableRecord       m_summary;        // summary

public:
   //--- constructor/destructor
                     COrdersHistory(void);
   virtual          ~COrdersHistory(void);
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
   MTAPIRES          TableWriteSummary(void);
   //--- clear
   void              Clear(void);
  };
//+------------------------------------------------------------------+
