//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportCacheKeysTime.h"
#include "..\Tools\DatasetField.h"
#include "..\Tools\CurrencyConverterCache.h"
//+------------------------------------------------------------------+
//| User cache class                                                 |
//+------------------------------------------------------------------+
class CUserCache : public CReportCacheKeysTime
  {
public:
   //--- User structure
   //--- Update version constant below when change this structure!
   #pragma pack(push,1)
   struct UserCache
     {
      UINT64            client_id;              // client id
      INT64             registration;           // registration time
      UINT              name;                   // name pos in dictionary (valid in full cache mode only, when m_client==false)
      UINT              currency;               // currency pos in dictionary
      UINT              lead_source;            // lead source pos in dictionary
      UINT              lead_campaign;          // lead campaign pos in dictionary
      UINT              country;                // country pos in dictionary (valid in full cache mode only, when m_client==false)
     };
   #pragma pack(pop)

private:
   //--- Cache version constant
   //--- Update it when changing structure above!
   static const UINT s_cache_version=3;
   //--- User record
   #pragma pack(push,1)
   struct UserRecord
     {
      UINT64            login;                  // login
      UINT64            client_id;              // client id
      INT64             registration;           // registration time
      wchar_t           group[64];              // group
      wchar_t           lead_source[32];        // lead source
      wchar_t           lead_campaign[32];      // lead campaign
      wchar_t           country[32];            // country (used in full cache mode only, when m_client==false)
      wchar_t           name[128];              // name (used in full cache mode only, when m_client==false)
     };
   #pragma pack(pop)

private:
   //--- cache context class
   class CUserCacheContext : public CReportCacheKeysTimeContext
     {
   public:
      IMTReportCacheKeySet *m_logins;           // user logins set

   public:
                        CUserCacheContext(IMTReportAPI &api,const bool client);
      virtual          ~CUserCacheContext(void);
     };

private:
   const bool        m_client;                  // flag determines that cache users with client attached only
   IMTReportCacheKeySet &m_logins;              // user logins set
   //--- static data
   static DatasetField s_fields[];              // request dataset fields descriptions
   static const UINT s_request_limit=100000;    // users request limit

public:
   //--- create class object
   static CUserCache* Create(IMTReportAPI &api,CReportParameter &params,CCurrencyConverterCache *currency,const bool client,MTAPIRES &res);
   //--- update cache
   MTAPIRES          Update(const INT64 from,const INT64 to);
   //--- update and filter cache
   MTAPIRES          UpdateFilter(const INT64 from,const INT64 to);
   //--- user logins key set
   const IMTReportCacheKeySet& Logins(void) const                                   { return(m_logins); }
   //--- user read from cache template methods
   template<class Receiver>
   MTAPIRES          ReadAll(Receiver &receiver,MTAPIRES(Receiver::*method)(const UINT64,const UserCache&)) { return(Read(m_logins,receiver,method)); }
   template<class Receiver>
   MTAPIRES          ReadAll(Receiver &receiver)                                    { return(ReadAll(receiver,&Receiver::UserRead)); }
   //--- read user by login
   MTAPIRES          ReadUser(const UINT64 login,const UserCache *&user)            { return(ReadData(login,(const void*&)user,sizeof(*user))); }
   //--- read user strings
   MTAPIRES          ReadName(const UserCache &user,LPCWSTR &name) const            { return(ReadDictionaryString(offsetof(UserCache,name),user.name,name));             }
   MTAPIRES          ReadCurrency(const UserCache &user,LPCWSTR &currency) const    { return(ReadDictionaryString(offsetof(UserCache,currency),user.currency,currency)); }
   MTAPIRES          ReadLeadSource(const UINT pos,LPCWSTR &string) const           { return(ReadDictionaryString(offsetof(UserCache,lead_source),pos,string));          }
   MTAPIRES          ReadLeadCampaign(const UINT pos,LPCWSTR &string) const         { return(ReadDictionaryString(offsetof(UserCache,lead_campaign),pos,string));        }
   MTAPIRES          ReadCountry(const UINT pos,LPCWSTR &string) const              { return(ReadDictionaryString(offsetof(UserCache,country),pos,string));              }
   //--- read user strings positions
   MTAPIRES          ReadLeadSourcePos(LPCWSTR lead_source,UINT &pos) const         { return(ReadDictionaryPos(offsetof(UserCache,lead_source),lead_source,pos));        }
   MTAPIRES          ReadLeadCampaignPos(LPCWSTR lead_campaign,UINT &pos) const     { return(ReadDictionaryPos(offsetof(UserCache,lead_campaign),lead_campaign,pos));    }

private:
                     CUserCache(CUserCacheContext &context,const bool client);
   virtual          ~CUserCache(void);
   //--- write users to cache
   virtual MTAPIRES  WriteData(const IMTDataset &users,UINT64 &id_last) override;
   //--- request users from base
   virtual MTAPIRES  SelectData(IMTDataset &users,IMTDatasetRequest &request,INT64 from,INT64 to,UINT64 id_from) override;
   //--- write missing keys
   virtual MTAPIRES  WriteMissingKeys(const IMTReportCacheKeySet &keys) override { return(m_client ? MT_RET_OK : CReportCacheKeysTime::WriteMissingKeys(keys)); }
   //--- prepare users request
   virtual MTAPIRES  PrepareRequest(IMTDatasetRequest &request,const IMTReportCacheKeySet &keys) override;
   //--- fill users request
   MTAPIRES          UserRequest(IMTDatasetRequest &request,INT64 from,INT64 to,UINT64 id_from);
   //--- group currency
   MTAPIRES          GroupCurrency(IMTConGroup &config,const wchar_t (&group)[64],CMTStr32 &currency);
   //--- write user to cache
   MTAPIRES          WriteUser(const UserRecord &user,const CMTStr32 &currency);
   //--- logging
   virtual MTAPIRES  LogSelectError(const MTAPIRES res,const UINT keys,const INT64 from,const INT64 to,const UINT64 id_from) override
     {
      return(LogErrorTime(res,L"Accounts selection by %u logins from #%I64u with time",from,to,keys,id_from));
     }
   virtual MTAPIRES  LogWriteError(const MTAPIRES res,const INT64 from,const INT64 to) override
     {
      return(LogErrorTime(res,L"Accounts caching with time",from,to));
     }
   virtual MTAPIRES  LogWriteOk(const INT64 from,const INT64 to,const UINT64 total,const UINT keys,const UINT time_select,const UINT time_write) override
     {
      return(LogOkTime(L"Selected %I64u accounts by %u logins in %u ms, cached in %u ms, time",from,to,total,keys,time_select,time_write));
     }
  };
//+------------------------------------------------------------------+
