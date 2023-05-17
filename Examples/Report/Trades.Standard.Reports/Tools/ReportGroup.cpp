//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "ReportGroup.h"
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CReportGroup::CReportGroup(const MTReportInfo &info) :
   CTradeReport(info),m_report_logins(nullptr),m_user_logins(nullptr),m_user_valid(false)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CReportGroup::~CReportGroup(void)
  {
   Clear();
  }
//+------------------------------------------------------------------+
//| Clear                                                            |
//+------------------------------------------------------------------+
void CReportGroup::Clear(void)
  {
//--- report logins interface
   if(m_report_logins)
     {
      m_report_logins->Release();
      m_report_logins=nullptr;
     }
//--- user logins interface
   if(m_user_logins)
     {
      m_user_logins->Release();
      m_user_logins=nullptr;
     }
   m_user_valid=false;
  }
//+------------------------------------------------------------------+
//| Report preparation                                               |
//+------------------------------------------------------------------+
MTAPIRES CReportGroup::Prepare(void)
  {
//--- checks
   if(!m_api)
      return(MT_RET_ERR_PARAMS);
//--- create interfaces
   if(!(m_report_logins=m_api->KeySetCreate()) ||
      !(m_user_logins=m_api->KeySetCreate()))
      return(MT_RET_ERR_MEM);
//--- get report request logins
   return(m_api->KeySetParamLogins(m_report_logins));
  }
//+------------------------------------------------------------------+
//| Read logins from dataset                                         |
//+------------------------------------------------------------------+
MTAPIRES CReportGroup::ReadLogins(IMTReportCacheKeySet &logins,IMTDataset &dataset,UINT64 &login_last)
  {
//--- clear logins
   MTAPIRES res=logins.Clear();
   if(res!=MT_RET_OK)
      return(res);
//--- reserve logins
   const UINT total=dataset.RowTotal();
   if(!total)
      return(MT_RET_OK);
   if((res=logins.Reserve(total))!=MT_RET_OK)
      return(res);
//--- iterate dataset
   for(UINT pos=0;pos<total;pos++)
     {
      //--- read login
      UINT64 login=0;
      if((res=dataset.RowRead(pos,&login,sizeof(UINT64)))!=MT_RET_OK)
         return(res);
      //--- store last login
      if(login_last<login)
         login_last=login;
      //--- add login to set
      if((res=logins.Insert(login))!=MT_RET_OK)
         return(res);
     }
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Read user from dataset                                           |
//+------------------------------------------------------------------+
MTAPIRES CReportGroup::ReadUser(IMTDataset &dataset,const UINT64 login,void *data,const UINT size)
  {
//--- check pointers
   if(!m_user_logins || !data || !size)
      return(MT_RET_ERR_PARAMS);
//--- search login
   const UINT64 *ptr=m_user_logins->Search(login);
   if(!ptr)
     {
      //--- user not found
      m_user_valid=false;
      return(MT_RET_OK);
     }
//--- pointer to keys sorted array
   const UINT64 *begin=m_user_logins->Array();
   if(!begin || ptr<begin)
      return(MT_RET_ERROR);
//--- dataset row
   const UINT row=UINT(ptr-begin);
   MTAPIRES res=dataset.RowRead(row,data,size);
   if(res!=MT_RET_OK)
      return(res);
//--- compare logins
   if(*((const UINT64 *)data)!=login)
      return(MT_RET_ERROR);
//--- copy user name
   m_user_valid=true;
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
