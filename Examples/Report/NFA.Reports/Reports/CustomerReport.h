//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportContext.h"
#include "..\Tools\ReportColumn.h"
//+------------------------------------------------------------------+
//| Customer NFA Report                                              |
//+------------------------------------------------------------------+
class CCustomerReport : public IMTReportContext
  {
private:
   //--- column ids
   enum columns
     {
      COLUMN_BATCH_DATE            =1,
      COLUMN_CUSTACCT_ID           =2,
      COLUMN_FDM_ID                =3,
      COLUMN_CUST_NAME             =4,
      COLUMN_CUST_BIRH_DATE        =5,
      COLUMN_CUST_TYPE             =6,
      COLUMN_CUST_GROUP            =7,
      COLUMN_COUNTRY_TYPE          =8,
      COLUMN_NET_LIQUIDATING_VALUE =9,
      CULUMN_ACCT_OPEN_DATE        =10,
     };
   //--- table record
   #pragma pack(push,1)
   struct TableRecord
     {
      INT64             batch_date;
      UINT64            custacct_id;
      wchar_t           fdm_id[8];
      wchar_t           cust_name[256];
      INT64             cust_birth_date;
      wchar_t           cust_type[16];
      wchar_t           cust_group[32];
      wchar_t           country_type[16];
      double            net_liquidating_value;
      INT64             acct_open_date;
     };
   #pragma pack(pop)

private:
   static MTReportInfo s_info;            // report information     
   static ReportColumn s_columns[];       // column descriptions

public:
                     CCustomerReport(void);
   virtual          ~CCustomerReport(void);
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
