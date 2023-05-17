//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportCacheTime.h"
#include "..\Tools\DatasetField.h"
#include "..\Tools\CurrencyConverterCache.h"
//+------------------------------------------------------------------+
//| User group cache class                                           |
//+------------------------------------------------------------------+
class CUserGroupCache : public CReportCacheTime
  {
public:
   //--- User group structure
   //--- Update version constant below when change this structure!
   #pragma pack(push,1)
   struct UserGroup
     {
      UINT              currency;               // currency pos in dictionary
      UINT              group;                  // group pos in dictionary
     };
   #pragma pack(pop)

private:
   //--- Cache version constant
   //--- Update it when changing structure above!
   static const UINT s_cache_version=1;
   //--- User record
   #pragma pack(push,1)
   struct UserRecord
     {
      UINT64            login;                  // login
      wchar_t           group[64];              // group
     };
   #pragma pack(pop)

private:
   //--- static data
   static DatasetField s_fields[];              // request dataset fields descriptions
   static const UINT s_request_limit=100*1024;  // users selection request limit

public:
   //--- create class object
   static CUserGroupCache* Create(IMTReportAPI &api,CReportParameter &params,CCurrencyConverterCache &currency,MTAPIRES &res);
   //--- read user by login
   MTAPIRES          ReadUser(const UINT64 login,const UserGroup *&user)         { return(ReadData(login,(const void*&)user,sizeof(*user))); }
   //--- read user strings
   MTAPIRES          ReadCurrency(const UserGroup &user,LPCWSTR &currency) const { return(ReadDictionaryString(offsetof(UserGroup,currency),user.currency,currency)); }
   MTAPIRES          ReadGroup(const UINT pos,LPCWSTR &string) const             { return(ReadDictionaryString(offsetof(UserGroup,group),pos,string));                }

private:
   explicit          CUserGroupCache(CReportCacheContext &context);
   virtual          ~CUserGroupCache(void)=default;
   //--- request data from base
   virtual MTAPIRES  SelectData(IMTDataset &deals,INT64 from,INT64 to,UINT64 id_from) override;
   //--- write users to cache
   virtual MTAPIRES  WriteData(const IMTDataset &data,UINT64 &id_last) override;
   //--- fill users request
   MTAPIRES          UserRequest(IMTDatasetRequest &request,INT64 from,INT64 to,UINT64 id_from);
   //--- group currency
   MTAPIRES          GroupCurrency(IMTConGroup &config,const wchar_t (&group)[64],CMTStr32 &currency);
   //--- write user to cache
   MTAPIRES          WriteUser(const UserRecord &user,const CMTStr32 &currency);
   //--- logging
   virtual MTAPIRES  LogSelectError(const MTAPIRES res,const INT64 from,const INT64 to,const UINT64 id_from) override
     {
      return(LogErrorTime(res,L"Users selection from '%I64u' with registration time",from,to,id_from));
     }
   virtual MTAPIRES  LogWriteError(const MTAPIRES res,const INT64 from,const INT64 to) override
     {
      return(LogErrorTime(res,L"Users caching with registration time",from,to));
     }
   virtual MTAPIRES  LogWriteOk(const INT64 from,const INT64 to,const UINT64 total,const UINT time_select,const UINT time_write) override
     {
      return(LogOkTime(L"Selected %I64u users in %u ms, cached in %u ms, registration time",from,to,total,time_select,time_write));
     }
  };
//+------------------------------------------------------------------+
