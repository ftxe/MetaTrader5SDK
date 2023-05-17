//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "DepositCache.h"
//+------------------------------------------------------------------+
//| Deal request dataset fields descriptions                         |
//+------------------------------------------------------------------+
DatasetField CDepositCache::s_fields[]=
  {
   //--- id                                      , select , offset
     { IMTDatasetField::FIELD_DEAL_LOGIN           , true   , offsetof(DealRecord,login)           },
   { IMTDatasetField::FIELD_DEAL_TIME            , true   , offsetof(DealRecord,time)            },
   { IMTDatasetField::FIELD_DEAL_ACTION          },
   { IMTDatasetField::FIELD_DEAL_ENTRY           },
   { IMTDatasetField::FIELD_DEAL_PROFIT          , true   , offsetof(DealRecord,profit)          },
   { IMTDatasetField::FIELD_DEAL_DIGITS_CURRENCY , true   , offsetof(DealRecord,currency_digits) },
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CDepositCache::CDepositCache(CReportCacheContext &context) :
   CReportCacheSelectTime(context)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CDepositCache::~CDepositCache(void)
  {
  }
//+------------------------------------------------------------------+
//| create class object                                              |
//+------------------------------------------------------------------+
CDepositCache* CDepositCache::Create(IMTReportAPI &api,MTAPIRES &res)
  {
//--- create cache context
   CReportCacheContext context(api,L"Deposit_Cache",s_cache_version);
   if((res=context.Status())!=MT_RET_OK)
      return(nullptr);
//--- create deposit cache object
   CDepositCache *deposits=new(std::nothrow) CDepositCache(context);
   if(!deposits)
      res=MT_RET_ERR_MEM;
//--- return new deposit cache object
   return(deposits);
  }
//+------------------------------------------------------------------+
//| write deals to deposits                                          |
//+------------------------------------------------------------------+
MTAPIRES CDepositCache::WriteData(const IMTDataset &deals)
  {
//--- deals total
   const UINT total=deals.RowTotal();
   if(!total)
      return(MT_RET_OK);
//--- save current time
   const DWORD start_time_ms=timeGetTime();
//--- iterate deals
   DealRecord deal={0};
   for(UINT pos=0;pos<total;pos++)
     {
      //--- read deal
      MTAPIRES res=deals.RowRead(pos,&deal,sizeof(deal));
      if(res!=MT_RET_OK)
         return(res);
      //--- get user deposit
      UserDeposit deposit;
      const UserDeposit *prev_deposit=nullptr;
      res=ReadDeposit(deal.login,prev_deposit);
      if(res==MT_RET_OK && prev_deposit)
        {
         //--- copy deposit
         memcpy(&deposit,prev_deposit,sizeof(deposit));
         //--- add deposit
         deposit.DepositAdd(deal.profit,deal.time);
        }
      else
         if((res==MT_RET_OK && !prev_deposit) || res==MT_RET_OK_NONE)
           {
            //--- init deposit
            deposit.Init(deal.profit,deal.time,deal.currency_digits);
           }
         else
            return(res);
      //--- write user deposit
      if((res=m_cache.WriteValue(deal.login,&deposit,sizeof(deposit)))!=MT_RET_OK)
         return(res);
     }
//--- write statistics to log
   return(m_api.LoggerOut(MTLogOK,L"Cached %u users deposits in %u ms",total,timeGetTime()-start_time_ms));
  }
//+------------------------------------------------------------------+
//| Request deals from base                                          |
//+------------------------------------------------------------------+
MTAPIRES CDepositCache::SelectData(IMTDataset &deals,const IMTReportCacheKeySet &keys,const INT64 from,const INT64 to)
  {
//--- checks
   if(from>to)
      return(MT_RET_ERR_PARAMS);
//--- create request
   IMTDatasetRequest *request=m_api.DatasetRequestCreate();
   if(!request)
      return(MT_RET_ERR_MEM);
//--- fill request
   MTAPIRES res=DealRequest(*request,keys,from,to);
   if(res!=MT_RET_OK)
     {
      request->Release();
      return(res);
     }
//--- save current time
   const DWORD start_time_ms=timeGetTime();
//--- request from base
   if((res=m_api.DealSelect(request,&deals))!=MT_RET_OK)
      m_api.LoggerOut(MTLogAtt,L"Deals selection failed, error: %s",SMTFormat::FormatError(res));
   else
      res=m_api.LoggerOut(MTLogOK,L"Selected %u deals in %u ms",deals.RowTotal(),timeGetTime()-start_time_ms);
//--- release request
   request->Release();
//--- return result
   return(res);
  }
//+------------------------------------------------------------------+
//| Fill deals request                                               |
//+------------------------------------------------------------------+
MTAPIRES CDepositCache::DealRequest(IMTDatasetRequest &request,const IMTReportCacheKeySet &logins,const INT64 from,const INT64 to)
  {
//--- checks
   if(from>to)
      return(MT_RET_ERR_PARAMS);
//--- fill request
   MTAPIRES res=MT_RET_OK;
   CDatasetRequest composer(s_fields,_countof(s_fields),request);
   while(const UINT id=composer.Next(res))
      switch(id)
        {
         case IMTDatasetField::FIELD_DEAL_LOGIN:   res=composer.FieldAddWhereKeys(logins); break;
         case IMTDatasetField::FIELD_DEAL_TIME:    res=composer.FieldAddBetweenInt(from,to); break;
         case IMTDatasetField::FIELD_DEAL_ACTION:  res=composer.FieldAddWhereUInt(IMTDeal::DEAL_BALANCE); break;
         case IMTDatasetField::FIELD_DEAL_ENTRY:   res=composer.FieldAddWhereUInt(IMTDeal::ENTRY_IN); break;
         case IMTDatasetField::FIELD_DEAL_PROFIT:  res=composer.FieldAddBetweenDouble(DBL_EPSILON,DBL_MAX); break;
         default:                                  res=composer.FieldAdd(); break;
        }
//--- return result
   return(res);
  }
//+------------------------------------------------------------------+
//| User deposit constructor                                         |
//+------------------------------------------------------------------+
void CDepositCache::UserDeposit::Init(const double value,const INT64 ctm,const UINT _currency_digits)
  {
   amount         =value;
   count          =1;
   first_time     =ctm;
   first          =value;
   currency_digits=_currency_digits;
  }
//+------------------------------------------------------------------+
//| Deposit addition                                                 |
//+------------------------------------------------------------------+
void CDepositCache::UserDeposit::DepositAdd(const double value,const INT64 ctm)
  {
//--- add and count deposit
   amount=SMTMath::MoneyAdd(value,amount,currency_digits);
   count++;
//--- correct first time
   if(first_time>ctm)
      first_time=ctm;
  }
//+------------------------------------------------------------------+
