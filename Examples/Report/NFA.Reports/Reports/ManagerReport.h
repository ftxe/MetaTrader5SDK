//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportContext.h"
#include "..\Tools\ReportColumn.h"
//+------------------------------------------------------------------+
//| Manager NFA Report                                               |
//+------------------------------------------------------------------+
class CManagerReport : public IMTReportContext
  {
private:
   //--- column ids
   enum columns
     {
      COLUMN_BATCH_DATE            =1,
      COLUMN_MANAGER_ID            =2,
      COLUMN_NFA_ID                =3,
      COLUMN_FDM_ID                =4,
      COLUMN_MANAGER_NAME          =5,
      COLUMN_COUNTRY_TYPE          =6,
      COLUMN_START_DATE            =7,
     };
   //--- table record
   #pragma pack(push,1)
   struct TableRecord
     {
      INT64             batch_date;
      wchar_t           manager_id[32];
      wchar_t           nfa_id[8];
      wchar_t           fdm_id[8];
      wchar_t           manager_name[100];
      wchar_t           country_type[12];
      INT64             start_date;
     };
   #pragma pack(pop)

private:
   static MTReportInfo s_info;            // report information     
   static ReportColumn s_columns[];       // column descriptions

public:
   //---
                     CManagerReport(void);
   virtual          ~CManagerReport(void);
   //--- IMTReportContext implementation
   //--- get report information structure
   static MTAPIRES   Info(MTReportInfo &info);
   //--- release instance of report class
   virtual void      Release(void);
   //--- generate report
   virtual MTAPIRES  Generate(const UINT type,IMTReportAPI *api);

private:
   static bool       Fill(CTextReader &reader,TableRecord &record,const ReportContext &context);
  };
//+------------------------------------------------------------------+
