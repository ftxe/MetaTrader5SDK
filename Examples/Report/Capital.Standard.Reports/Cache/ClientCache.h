//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportCacheSelect.h"
#include "..\Tools\DatasetField.h"
//+------------------------------------------------------------------+
//| Client cache class                                               |
//+------------------------------------------------------------------+
class CClientCache : public CReportCacheSelect
  {
public:
   //--- Client structure
   //--- Update version constant below when change this structure!
   #pragma pack(push,1)
   struct ClientCache
     {
      //--- positions in dictionaries
      UINT              name;                   // client full name
      UINT              country;                // country
      UINT              lead_source;            // lead source
      UINT              lead_campaign;          // lead campaign
     };
   #pragma pack(pop)
   //--- Cache dictionaries Ids
   enum EnDictionaryId
     {
      DICTIONARY_ID_NAME         =offsetof(ClientCache,name),           // name
      DICTIONARY_ID_COUNTRY      =offsetof(ClientCache,country),        // country
      DICTIONARY_ID_LEAD_SOURCE  =offsetof(ClientCache,lead_source),    // lead source
      DICTIONARY_ID_LEAD_CAMPAIGN=offsetof(ClientCache,lead_campaign),  // lead campaign
     };

private:
   //--- Cache version constant
   //--- Update it when changing structure above!
   static const UINT s_cache_version=2;
   #pragma pack(push,1)
   //--- Client record
   struct ClientRecord
     {
      UINT64            client_id;              // client id
      wchar_t           first_name[32];         // first name
      wchar_t           middle_name[32];        // middle name
      wchar_t           second_name[32];        // second name
      wchar_t           country[64];            // country
      wchar_t           lead_source[64];        // lead source
      wchar_t           lead_campaign[64];      // lead campaign
     };
   #pragma pack(pop)

private:
   //--- static data
   static DatasetField s_fields[];              // request dataset fields descriptions

public:
   //--- create class object
   static CClientCache* Create(IMTReportAPI &api,MTAPIRES &res);
   //--- read client
   MTAPIRES          ReadClient(const UINT64 id,const ClientCache *&client) const      { return(CReportCache::ReadData(id,(const void*&)client,sizeof(*client))); }
   //--- read client strings
   MTAPIRES          ReadName(const ClientCache &client,LPCWSTR &name) const           { return(ReadDictionaryString(DICTIONARY_ID_NAME,client.name,name)); }
   MTAPIRES          ReadCountry(const UINT pos,LPCWSTR &string) const                 { return(ReadDictionaryString(DICTIONARY_ID_COUNTRY,pos,string)); }
   MTAPIRES          ReadLeadSource(const UINT pos,LPCWSTR &string) const              { return(ReadDictionaryString(DICTIONARY_ID_LEAD_SOURCE,pos,string)); }
   MTAPIRES          ReadLeadCampaign(const UINT pos,LPCWSTR &string) const            { return(ReadDictionaryString(DICTIONARY_ID_LEAD_CAMPAIGN,pos,string)); }
   //--- read user strings positions
   MTAPIRES          ReadLeadSourcePos(LPCWSTR lead_source,UINT &pos) const            { return(ReadDictionaryPos(DICTIONARY_ID_LEAD_SOURCE,lead_source,pos)); }
   MTAPIRES          ReadLeadCampaignPos(LPCWSTR lead_campaign,UINT &pos) const        { return(ReadDictionaryPos(DICTIONARY_ID_LEAD_CAMPAIGN,lead_campaign,pos)); }

private:
   explicit          CClientCache(CReportCacheContext &context);
   virtual          ~CClientCache(void);
   //--- write deals to deposits
   virtual MTAPIRES  WriteData(const IMTDataset &clients) override;
   //--- request deals from base
   virtual MTAPIRES  SelectData(IMTDataset &clients,const IMTReportCacheKeySet &keys) override;
   //--- fill deals request
   MTAPIRES          ClientRequest(IMTDatasetRequest &request,const IMTReportCacheKeySet &ids);
   //--- write client to cache
   MTAPIRES          WriteClient(const ClientRecord &client);
   //--- client full name formatting
   static LPCWSTR    FormatFullName(CMTStr& str,const wchar_t (&name)[32],const wchar_t (&last_name)[32],const wchar_t (&middle_name)[32]);
   static LPCWSTR    FormatFullName(CMTStr& str,const ClientRecord &client) { return(FormatFullName(str,client.first_name,client.second_name,client.middle_name)); }
  };
//+------------------------------------------------------------------+
