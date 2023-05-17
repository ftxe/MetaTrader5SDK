//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportCacheChild.h"
#include "UserCache.h"
#include "UserFilter.h"
//+------------------------------------------------------------------+
//| Client by user cache class                                       |
//+------------------------------------------------------------------+
class CClientUserCache : public CReportCacheChild
  {
public:
   //--- Client structure
   //--- Update version constant below when change this structure!
   #pragma pack(push,1)
   struct ClientUserCache
     {
      INT64             registration;           // registration time
     };
   #pragma pack(pop)

private:
   //--- Cache version constant
   //--- Update it when changing structure above!
   static const UINT s_cache_version=2;
   //--- cache context class
   class CClientUserCacheContext : public CReportCacheChildContext
     {
   public:
      IMTReportCacheKeySet *m_ids;              // client ids key set
      IMTReportCacheKeySet *m_logins;           // user logins key set

   public:
      explicit          CClientUserCacheContext(IMTReportAPI &api);
      virtual          ~CClientUserCacheContext(void);
     };

private:
   IMTReportCacheKeySet &m_ids;                 // client ids key set
   IMTReportCacheKeySet &m_logins;              // user logins key set
   CUserCache       &m_users;                   // user cache reference
   const IMTReportCacheKeySet *m_keys_update;   // update keys
   INT64             m_filter_from;             // filter time from
   INT64             m_filter_to;               // filter time to

public:
   //--- create class object
   static CClientUserCache* Create(IMTReportAPI &api,CUserCache &users,MTAPIRES &res);
   //--- update cache
   MTAPIRES          Update(const INT64 from,const INT64 to);
   //--- client ids key set
   const IMTReportCacheKeySet& Ids(void) const                                            { return(m_ids); }
   //--- user logins key set
   const IMTReportCacheKeySet& Logins(void) const                                         { return(m_logins); }
   //--- read client
   MTAPIRES          ReadClient(const UINT64 id,const ClientUserCache *&client) const     { return(CReportCache::ReadData(id,(const void*&)client,sizeof(*client))); }

private:
                     CClientUserCache(CClientUserCacheContext &context,CUserCache &users);
   virtual          ~CClientUserCache(void);
   //--- write cache keys
   virtual MTAPIRES  WriteKeys(const IMTReportCacheKeySet &keys,const IMTReportCacheKeySet &keys_parent) override;
   //--- user read handler
   MTAPIRES          UserRead(const UINT64 login,const CUserCache::UserCache &user);
   //--- filter cache
   MTAPIRES          Filter(const IMTReportCacheKeySet &ids,const INT64 from,const INT64 to);
   //--- filter client by report parameters
   MTAPIRES          FilterClient(const UINT64 id,const ClientUserCache &client);
  };
//+------------------------------------------------------------------+
