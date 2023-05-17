//+------------------------------------------------------------------+
//|                               MetaTrader 5 Daily.Standard.Report |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportSelect.h"
#include "DatasetField.h"
//+------------------------------------------------------------------+
//| Select history base class                                        |
//+------------------------------------------------------------------+
class CHistorySelectBase : public CReportSelect
  {
   //--- API interfaces
   IMTDatasetField  *m_field_time;           // request time field
   IMTDatasetField  *m_field_login;          // request login field
   //--- request time
   INT64             m_from;                 // from
   INT64             m_to;                   // to
   INT64             m_month_end;            // month end
   bool              m_full;                 // full result flag

public:
   //--- constructor/destructor
                     CHistorySelectBase(LPCWSTR log_prefix);
   virtual          ~CHistorySelectBase(void);
   //--- clear
   virtual void      Clear(void) override;
   //--- request fields
   MTAPIRES          FieldTime(IMTDatasetField *field_time);
   MTAPIRES          FieldLogin(IMTDatasetField *field_login);
   //--- select hsitory by time and logins
   MTAPIRES          Select(INT64 from,INT64 to,const IMTReportCacheKeySet *logins);
   //--- select next part
   IMTDataset*       Next(MTAPIRES &res,UINT64 &login);

protected:
   //--- prepare
   MTAPIRES          Prepare(IMTReportAPI *api,const DatasetField *fields,UINT fields_total,UINT limit,UINT field_login,UINT field_time);
   //--- overridable select method
   virtual MTAPIRES  DoSelect(IMTReportAPI &api,IMTDatasetRequest &request,IMTDataset &dataset)=0;
   //--- end of month
   static INT64      MonthEnd(INT64 ctm,INT64 to);
  };
//+------------------------------------------------------------------+
//| Order history select                                             |
//+------------------------------------------------------------------+
class CHistorySelect : public CHistorySelectBase
  {
public:
                     CHistorySelect(void) : CHistorySelectBase(L"History ") {}

protected:
   //--- select method
   virtual MTAPIRES  DoSelect(IMTReportAPI &api,IMTDatasetRequest &request,IMTDataset &dataset) { return(api.HistorySelect(&request,&dataset)); }
  };
//+------------------------------------------------------------------+
//| Deal history select                                              |
//+------------------------------------------------------------------+
class CDealSelect : public CHistorySelectBase
  {
public:
                     CDealSelect(void) : CHistorySelectBase(L"Deals ") {}

protected:
   //--- select method
   virtual MTAPIRES  DoSelect(IMTReportAPI &api,IMTDatasetRequest &request,IMTDataset &dataset) { return(api.DealSelect(&request,&dataset)); }
  };
//+------------------------------------------------------------------+
//| Daily select                                                     |
//+------------------------------------------------------------------+
class CDailySelect : public CHistorySelectBase
  {
public:
                     CDailySelect(void) : CHistorySelectBase(L"Daily ") {}

   MTAPIRES          Prepare(IMTReportAPI *api,const DatasetField *fields,const UINT fields_total,const UINT limit)
     {
      return(CHistorySelectBase::Prepare(api,fields,fields_total,limit,IMTDatasetField::FIELD_DAILY_LOGIN,IMTDatasetField::FIELD_DAILY_DATE_TIME));
     }

protected:
   //--- select method
   virtual MTAPIRES  DoSelect(IMTReportAPI &api,IMTDatasetRequest &request,IMTDataset &dataset) { return(api.DailySelect(&request,&dataset)); }
  };
//+------------------------------------------------------------------+

