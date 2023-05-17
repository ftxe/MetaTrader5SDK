//+------------------------------------------------------------------+
//|                                     MetaTrader 5 Traders.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "DealUserCache.h"
//+------------------------------------------------------------------+
//| Deal request dataset fields descriptions                         |
//+------------------------------------------------------------------+
const DatasetField CDealUserCache::s_fields[]=
   //--- id                                      , select , offset
  {{ IMTDatasetField::FIELD_DEAL_LOGIN           , true   , offsetof(DealRecord,login)          },
   { IMTDatasetField::FIELD_DEAL_TIME           },
   { IMTDatasetField::FIELD_DEAL_ACTION          , true   , offsetof(DealRecord,action)         },
   { IMTDatasetField::FIELD_DEAL_ENTRY           , true   , offsetof(DealRecord,entry)          },
   { IMTDatasetField::FIELD_DEAL_PRICE           , true   , offsetof(DealRecord,price)          },
   { IMTDatasetField::FIELD_DEAL_PRICE_POSITION  , true   , offsetof(DealRecord,price_position) },
   { IMTDatasetField::FIELD_DEAL_PRICE_SL        , true   , offsetof(DealRecord,sl)             },
   { IMTDatasetField::FIELD_DEAL_PRICE_TP        , true   , offsetof(DealRecord,tp)             },
  };
//--- deal request actions and entries
const UINT64 CDealUserCache::s_actions[]={ IMTDeal::DEAL_BUY, IMTDeal::DEAL_SELL };
const UINT64 CDealUserCache::s_entries[]={ IMTDeal::ENTRY_IN, IMTDeal::ENTRY_OUT, IMTDeal::ENTRY_INOUT };
//+------------------------------------------------------------------+
//| create class object                                              |
//+------------------------------------------------------------------+
CDealUserCache* CDealUserCache::Create(IMTReportAPI &api,const INT64 from,const INT64 to,MTAPIRES &res)
  {
//--- calculate time hash
   const UINT64 time_hash=TimeHash(from,to);
//--- make cache name
   CMTStr128 name;
   name.Format(L"Deal_User_Cache_%.016llx",time_hash);
//--- create cache context
   CReportCacheKeysTimeContext context(api,name.Str(),s_cache_version);
   if((res=context.Status())!=MT_RET_OK)
      return(nullptr);
//--- create deals statistics cache object
   CDealUserCache *cache=new(std::nothrow) CDealUserCache(context);
   if(!cache)
      res=MT_RET_ERR_MEM;
//--- return new deals statistics cache object
   return(cache);
  }
//+------------------------------------------------------------------+
//| write deals to deals aggregates                                  |
//+------------------------------------------------------------------+
MTAPIRES CDealUserCache::WriteData(const IMTDataset &deals,UINT64 &id_last)
  {
//--- deals total
   const UINT total=deals.RowTotal();
   if(!total)
      return(MT_RET_OK);
//--- iterate deals
   CDealStatWrite stat(m_cache);
   DealRecord deal{};
   for(UINT pos=0;pos<total;pos++)
     {
      //--- read deal
      MTAPIRES res=deals.RowRead(pos,&deal,sizeof(deal));
      if(res!=MT_RET_OK)
         return(LogError(res,L"read deal %u of %u",pos,total));
      //--- store last deal login
      if(id_last<deal.login)
         id_last=deal.login;
      //--- check last login
      if(stat.Login()!=deal.login)
        {
         //--- write deals statistics to cache
         res=stat.Write();
         if(res!=MT_RET_OK && res!=MT_RET_OK_NONE)
            return(LogError(res,L"write deals statistics %u of %u for %I64u to cache",pos,total,deal.login));
         //--- get deals statistics
         const DealStat *read=nullptr;
         res=ReadStat(deal.login,read);
         if(res!=MT_RET_OK && res!=MT_RET_OK_NONE)
            return(LogError(res,L"get %I64u account deals statistics %u of %u",deal.login,pos,total));
         //--- initialize deals statistics writer
         stat.Init(deal.login,read);
        }
      //--- add deals statistics with deal to cache
      stat.AddDeal(deal);
     }
//--- write deals statistics to cache
   const MTAPIRES res=stat.Write();
   if(res!=MT_RET_OK && res!=MT_RET_OK_NONE)
      return(LogError(res,L"write %u deals statistics to cache",total));
   return(res);
  }
//+------------------------------------------------------------------+
//| Prepare deals request                                            |
//+------------------------------------------------------------------+
MTAPIRES CDealUserCache::PrepareRequest(IMTDatasetRequest &request,const IMTReportCacheKeySet &keys)
  {
//--- fill request
   MTAPIRES res=MT_RET_OK;
   CDatasetRequest composer(s_fields,_countof(s_fields),request);
   while(const UINT id=composer.Next(res))
      switch(id)
        {
         case IMTDatasetField::FIELD_DEAL_LOGIN:   res=composer.FieldAddWhereKeys(keys); break;
         case IMTDatasetField::FIELD_DEAL_ACTION:  res=composer.FieldAddWhereUIntArray(s_actions,_countof(s_actions)); break;
         case IMTDatasetField::FIELD_DEAL_ENTRY:   res=composer.FieldAddWhereUIntArray(s_entries,_countof(s_entries)); break;
         default:                                  res=composer.FieldAdd(); break;
        }
//--- limit the number of rows of the resulting dataset
   request.RowLimit(s_request_limit);
//--- return result
   return(res);
  }
//+------------------------------------------------------------------+
//| Fill deals request                                               |
//+------------------------------------------------------------------+
MTAPIRES CDealUserCache::DealRequest(IMTDatasetRequest &request,const INT64 from,const INT64 to,const UINT64 id_from)
  {
//--- checks
   if(from>to)
      return(MT_RET_ERR_PARAMS);
//--- update request
   MTAPIRES res=MT_RET_OK;
   CDatasetRequest composer(s_fields,_countof(s_fields),request);
   while(const UINT id=composer.Next(res))
      switch(id)
        {
         case IMTDatasetField::FIELD_DEAL_LOGIN:   res=composer.FieldSetFromUInt(id_from); break;
         case IMTDatasetField::FIELD_DEAL_TIME:    res=composer.FieldSetBetweenInt(from,to); break;
        }
   return(res);
  }
//+------------------------------------------------------------------+
//| Calculate time hash                                              |
//+------------------------------------------------------------------+
UINT64 CDealUserCache::TimeHash(const INT64 from,const INT64 to)
  {
//--- fastcache64 used
   const UINT64 m=0x880355f21e6d1965ULL;
   UINT64 hash=0;
//--- iterate all keys
   hash^=HashMix(UINT64(from));
   hash*=m;
   hash^=HashMix(UINT64(to));
   hash*=m;
   return(HashMix(hash));
  }
//+------------------------------------------------------------------+
//| Compression function for Merkle-Damgard construction             |
//+------------------------------------------------------------------+
UINT64 CDealUserCache::HashMix(UINT64 h)
  {
   h^=h>>23;
   h*=0x2127599bf4325c37ULL;
   h^=h>>47;
   return(h);
  }
//+------------------------------------------------------------------+
//| Initialize deal statistics writer                                |
//+------------------------------------------------------------------+
void CDealUserCache::CDealStatWrite::Init(const UINT64 login,const DealStat *stat)
  {
//--- store login
   m_login=login;
//--- copy or clear intial statistics
   if(stat)
      memcpy(&m_stat,stat,sizeof(m_stat));
   else
      ZeroMemory(&m_stat,sizeof(m_stat));
  }
//+------------------------------------------------------------------+
//| Write deal statistics to cache                                   |
//+------------------------------------------------------------------+
MTAPIRES CDealUserCache::CDealStatWrite::Write(void)
  {
//--- check login
   if(!m_login)
      return(MT_RET_OK_NONE);
//--- check empty statistics
   if(!m_stat.loss.ordered.count && !m_stat.loss.actual.count && !m_stat.profit.ordered.count && !m_stat.profit.actual.count)
      return(MT_RET_OK_NONE);
//--- write buffer to cache
   const MTAPIRES res=m_cache.WriteValue(m_login,&m_stat,sizeof(m_stat));
//--- clear login
   m_login=0;
   return(res);
  }
//+------------------------------------------------------------------+
//| Add deal to cache writer                                         |
//+------------------------------------------------------------------+
void CDealUserCache::CDealStatWrite::AddDeal(const DealRecord &deal)
  {
//--- deal action and entry
   bool buy=deal.action==IMTDeal::DEAL_BUY;
   const bool inout=deal.entry==IMTDeal::ENTRY_INOUT;
   if((inout || deal.entry==IMTDeal::ENTRY_IN) && fabs(deal.price)>=DBL_EPSILON)
     {
      //--- aggregate stop levels
      AddPriceDiffCheck(m_stat.loss.ordered,deal.sl,deal.price,!buy);
      AddPriceDiffCheck(m_stat.profit.ordered,deal.tp,deal.price,buy);
     }
   if((inout || deal.entry==IMTDeal::ENTRY_OUT) && fabs(deal.price_position)>=DBL_EPSILON)
     {
      //--- opposit direction of deal
      buy=!buy;
      //--- aggregate position price
      const double diff=deal.price-deal.price_position;
      if((diff<0.0)==buy)
         AddPriceDiff(m_stat.loss.actual,-diff/deal.price_position,buy);
      else
         AddPriceDiff(m_stat.profit.actual,diff/deal.price_position,buy);
     }
  }
//+------------------------------------------------------------------+
//| Add price difference to deal statistics aggregate                |
//+------------------------------------------------------------------+
void CDealUserCache::CDealStatWrite::AddPriceDiffCheck(DealPrice &aggregate,const double price,const double base,const bool buy)
  {
   if(fabs(price)<DBL_EPSILON)
      return;
   AddPriceDiff(aggregate,(price-base)/base,buy);
  }
//+------------------------------------------------------------------+
//| Add price difference to deal statistics aggregate                |
//+------------------------------------------------------------------+
void CDealUserCache::CDealStatWrite::AddPriceDiff(DealPrice &aggregate,const double diff,const bool buy)
  {
   aggregate.count++;
   if(buy)
      aggregate.price_diff+=diff;
   else
      aggregate.price_diff-=diff;
  }
//+------------------------------------------------------------------+
