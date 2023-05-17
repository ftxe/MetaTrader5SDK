//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportSelect.h"
#include "DatasetField.h"
//+------------------------------------------------------------------+
//| Select by gropups base class                                     |
//+------------------------------------------------------------------+
class CGroupSelectBase : public CReportSelect
  {
   //--- API interfaces
   IMTDatasetField  *m_field_time;           // request time field
   IMTDatasetField  *m_field_login;          // request login field
   IMTDatasetField  *m_field_group;          // request group field
   //--- request time
   INT64             m_from;                 // from
   INT64             m_to;                   // to
   bool              m_full;                 // full result flag

public:
   //--- constructor/destructor
                     CGroupSelectBase(LPCWSTR log_prefix);
   virtual          ~CGroupSelectBase(void);
   //--- clear
   virtual void      Clear(void) override;
   //--- request fields
   MTAPIRES          FieldTime(IMTDatasetField *field_time);
   MTAPIRES          FieldLogin(IMTDatasetField *field_login);
   MTAPIRES          FieldGroup(IMTDatasetField *field_group);
   //--- select users by group
   MTAPIRES          Select(LPCWSTR group);
   //--- select next part
   IMTDataset*       Next(MTAPIRES &res,UINT64 login);
   //--- select users by logins
   IMTDataset*       SelectUnlimited(MTAPIRES &res,const IMTReportCacheKeySet *logins);

protected:
   //--- prepare
   MTAPIRES          Prepare(IMTReportAPI *api,const DatasetField *fields,UINT fields_total,UINT limit,UINT field_login,UINT field_group);
   //--- overridable select method
   virtual MTAPIRES  DoSelect(IMTReportAPI &api,IMTDatasetRequest &request,IMTDataset &dataset)=0;
  };
//+------------------------------------------------------------------+
//| User select                                                      |
//+------------------------------------------------------------------+
class CUserSelect : public CGroupSelectBase
  {
public:
                     CUserSelect(void) : CGroupSelectBase(L"Users ") {}

   MTAPIRES          Prepare(IMTReportAPI *api,const DatasetField *fields,const UINT fields_total,const UINT limit)
     {
      return(CGroupSelectBase::Prepare(api,fields,fields_total,limit,IMTDatasetField::FIELD_USER_LOGIN,IMTDatasetField::FIELD_USER_GROUP));
     }

protected:
   //--- select method
   virtual MTAPIRES  DoSelect(IMTReportAPI &api,IMTDatasetRequest &request,IMTDataset &dataset) { return(api.UserSelect(&request,&dataset)); }
  };
//+------------------------------------------------------------------+

