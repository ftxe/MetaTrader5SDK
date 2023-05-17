//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "UserCache.h"
//+------------------------------------------------------------------+
//| User filter class                                                |
//+------------------------------------------------------------------+
class CUserFilter
  {
protected:
   const INT64       m_from;                       // time range begin
   const INT64       m_to;                         // time range end
   IMTReportCacheKeySet *m_keys;                   // user logins set
   IMTReportAPI     &m_api;                        // report api reference

private:
   const CUserCache &m_cache;                      // user cache reference
   IMTConParam      *m_param_lead_source;          // lead source parameter
   IMTConParam      *m_param_lead_campaign;        // lead campaign parameter
   INT64             m_lead_source;                // lead source string
   INT64             m_lead_campaign;              // lead campaign string

public:
                     CUserFilter(IMTReportAPI &api,const CUserCache &cache,const INT64 from,const INT64 to);
   virtual          ~CUserFilter(void);
   //--- user logins key set
   const IMTReportCacheKeySet* Logins(void) const                                   { return(m_keys); }
   //--- swap logins key set
   MTAPIRES          LoginsSwap(IMTReportCacheKeySet &logins);
   //--- user read handler
   MTAPIRES          UserRead(const UINT64 login,const CUserCache::UserCache &user);

protected:
   //--- initialization
   MTAPIRES          Initialize(void);

private:
   //--- match user with filter
   bool              UserMatch(const CUserCache::UserCache &user) const;
   //--- get report parameter string value
   LPCWSTR           ParamGetString(LPCWSTR name,IMTConParam *param,LPCWSTR def) const;
  };
//+------------------------------------------------------------------+
//| User filter class with clients                                   |
//+------------------------------------------------------------------+
class CUserFilterWithClients : public CUserFilter
  {
private:
   IMTReportCacheKeySet *m_clients;          // client id set

public:
                     CUserFilterWithClients(IMTReportAPI &api,const CUserCache &cache,const INT64 from,const INT64 to);
   virtual          ~CUserFilterWithClients(void);
   //--- user read handler
   MTAPIRES          UserRead(const UINT64 login,const CUserCache::UserCache &user);
   //--- user clients ids key set
   const IMTReportCacheKeySet* Clients(void) const                                   { return(m_clients); }

protected:
   //--- initialization
   MTAPIRES          Initialize(void);
  };
//+------------------------------------------------------------------+
//| User filter class by clients                                     |
//+------------------------------------------------------------------+
class CUserFilterByClients
  {
private:
   const INT64       m_from;                 // time range begin
   const IMTReportCacheKeySet &m_clients;    // client ids set
   IMTReportCacheKeySet &m_logins;           // user logins set

public:
                     CUserFilterByClients(const INT64 from,const IMTReportCacheKeySet &clients,IMTReportCacheKeySet &ids);
                    ~CUserFilterByClients(void) {}
   //--- user read handler
   MTAPIRES          UserRead(const UINT64 login,const CUserCache::UserCache &user);
  };
//+------------------------------------------------------------------+
