//+------------------------------------------------------------------+
//|                           MetaTrader 5 Accounts.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "UserSelect.h"
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CGroupSelectBase::CGroupSelectBase(const LPCWSTR log_prefix) :
   m_api(nullptr),m_request(nullptr),m_field_time(nullptr),m_field_login(nullptr),m_field_group(nullptr),m_dataset(nullptr),
   m_log(log_prefix),m_from(0),m_to(0),m_full(false)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CGroupSelectBase::~CGroupSelectBase(void)
  {
   Clear();
  }
//+------------------------------------------------------------------+
//| Clear                                                            |
//+------------------------------------------------------------------+
void CGroupSelectBase::Clear(void)
  {
//--- reset api interface
   m_api=nullptr;
//--- request interface
   if(m_request)
     {
      m_request->Release();
      m_request=nullptr;
     }
//--- request time field interface
   if(m_field_time)
     {
      m_field_time->Release();
      m_field_time=nullptr;
     }
//--- request login field interface
   if(m_field_login)
     {
      m_field_login->Release();
      m_field_login=nullptr;
     }
//--- request group field interface
   if(m_field_group)
     {
      m_field_group->Release();
      m_field_group=nullptr;
     }
//--- dataset interface
   m_dataset=nullptr;
//--- clear logger
   m_log.Clear();
//--- reset time
   m_from=0;
   m_to=0;
   m_full=false;
  }
//+------------------------------------------------------------------+
//| Prepare request                                                  |
//+------------------------------------------------------------------+
MTAPIRES CGroupSelectBase::Prepare(IMTReportAPI *api,const DatasetField *fields,const UINT fields_total,const UINT limit,const UINT field_login,const UINT field_group)
  {
//--- checks
   if(!api || !fields || !fields_total)
      return(MT_RET_ERR_PARAMS);
//--- prepare account select object
   MTAPIRES res=Prepare(api);
   if(res!=MT_RET_OK)
      return(res);
   if(!m_request)
      return(MT_RET_ERROR);
//--- fill request
   CDatasetRequest composer(fields,fields_total,*m_request);
   while(const UINT id=composer.Next(res))
      if(id==field_login)
         res=FieldLogin(composer.FieldAddReference(res));
      else
         if(id==field_group)
            res=FieldGroup(composer.FieldAddReference(res));
         else
            res=composer.FieldAdd();
   if(res!=MT_RET_OK)
      return(res);
//--- limit the number of rows of the resulting dataset
   return(m_request->RowLimit(limit));
  }
//+------------------------------------------------------------------+
//| Report preparation                                               |
//+------------------------------------------------------------------+
MTAPIRES CGroupSelectBase::Prepare(IMTReportAPI *api)
  {
//--- checks
   if(!api)
      return(MT_RET_ERR_PARAMS);
//--- store api interface
   m_api=api;
//--- initialize logger
   m_log.Initialize(api);
//--- create interfaces
   if(!(m_request=m_api->DatasetRequestCreate()) ||
      !(m_dataset=m_api->DatasetAppend()))
      return(MT_RET_ERR_MEM);
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| user request time field                                          |
//+------------------------------------------------------------------+
MTAPIRES CGroupSelectBase::FieldTime(IMTDatasetField *field_time)
  {
//--- checks
   if(!field_time)
      return(MT_RET_ERR_PARAMS);
//--- store field
   m_field_time=field_time;
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| user request login field                                         |
//+------------------------------------------------------------------+
MTAPIRES CGroupSelectBase::FieldLogin(IMTDatasetField *field_login)
  {
//--- checks
   if(!field_login)
      return(MT_RET_ERR_PARAMS);
//--- store field
   m_field_login=field_login;
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| user request group field                                         |
//+------------------------------------------------------------------+
MTAPIRES CGroupSelectBase::FieldGroup(IMTDatasetField *field_group)
  {
//--- checks
   if(!field_group)
      return(MT_RET_ERR_PARAMS);
//--- store field
   m_field_group=field_group;
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Select users by group                                            |
//+------------------------------------------------------------------+
MTAPIRES CGroupSelectBase::Select(LPCWSTR group)
  {
//--- checks
   if(!group || !*group || !m_field_group)
      return(MT_RET_ERR_PARAMS);
//--- initialize time
   m_from=0;
   m_to=0;
   m_full=false;
//--- clear user request group field conditions
   MTAPIRES res=DatasetField::FieldClearConditions(*m_field_group,false);
   if(res!=MT_RET_OK)
      return(res);
//--- update user request group field condition
   if((res=m_field_group->WhereAddString(group))!=MT_RET_OK)
      return(res);
//--- update request time
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Select next part                                                 |
//+------------------------------------------------------------------+
IMTDataset* CGroupSelectBase::Next(MTAPIRES &res,const UINT64 login)
  {
//--- checks
   if(!m_api || !m_request || !m_field_login || !m_dataset)
     {
      res=MT_RET_ERR_PARAMS;
      return(nullptr);
     }
//--- check last full request
   if(m_full)
     {
      res=MT_RET_OK;
      return(nullptr);
     }
//--- update request login range
   if((res=m_field_login->BetweenUInt(login,UINT64_MAX))!=MT_RET_OK)
      return(nullptr);
//--- select
   res=DoSelect(*m_api,*m_request,*m_dataset);
//--- partial result
   m_full=res!=MT_RET_ERR_PARTIAL;
   if(m_full)
     {
      //--- handle error
      if(res!=MT_RET_OK && res!=MT_RET_OK_NONE)
        {
         if(m_from || m_to)
            m_log.ErrorTime(res,L"selection from '%I64u' with time",m_from,m_to,login);
         else
            m_log.Error(res,L"selection from '%I64u'",login);
         return(nullptr);
        }
     }
   else
      res=MT_RET_OK;
//--- return dataset
   return(m_dataset);
  }
//+------------------------------------------------------------------+
//| Select users by logins                                           |
//+------------------------------------------------------------------+
IMTDataset* CGroupSelectBase::SelectUnlimited(MTAPIRES &res,const IMTReportCacheKeySet &logins)
  {
//--- checks
   if(!m_api || !m_request || !m_dataset || !m_field_login)
     {
      res=MT_RET_ERR_PARAMS;
      return(nullptr);
     }
//--- update user request login set
   if((res=m_field_login->WhereUIntSet(logins.Array(),logins.Total()))!=MT_RET_OK)
      return(nullptr);
//--- reset rows limit of the resulting dataset
   if((res=m_request->RowLimit(UINT_MAX))!=MT_RET_OK)
      return(nullptr);
//--- select users
   res=DoSelect(*m_api,*m_request,*m_dataset);
//--- handle error
   if(res!=MT_RET_OK && res!=MT_RET_OK_NONE)
     {
      if(m_from || m_to)
         m_log.ErrorTime(res,L"selection with time",m_from,m_to);
      else
         m_log.Error(res,L"selection");
      return(nullptr);
     }
//--- return dataset
   return(m_dataset);
  }
//+------------------------------------------------------------------+
