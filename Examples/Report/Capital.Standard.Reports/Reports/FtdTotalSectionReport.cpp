//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "FtdTotalSectionReport.h"
//+------------------------------------------------------------------+
//| Plugin description structure                                     |
//+------------------------------------------------------------------+
MTReportInfo CFtdTotalSectionReport::s_info=
  {
   100,                                            // report version
   MTReportAPIVersion,                             // report API version
   MTReportInfo::IE_VERSION_ANY,                   // minimal IE version
   L"First Time Deposit",                          // report name
   L"Copyright 2000-2023, MetaQuotes Ltd.",      // copyright
   L"MetaTrader 5 Report API plug-in",             // description
   MTReportInfo::SNAPSHOT_NONE,                    // snapshot modes
   MTReportInfo::TYPE_DASHBOARD,                   // report types
   L"Capital",                                     // report category
                                                   // request parameters
     {{ MTReportParam::TYPE_GROUPS,    MTAPI_PARAM_GROUPS,           DEFAULT_GROUPS             },
      { MTReportParam::TYPE_DATE,      MTAPI_PARAM_FROM                                         },
      { MTReportParam::TYPE_DATE,      MTAPI_PARAM_TO                                           },
     },3,                                          // request parameters total
     {                                             // config parameters
      { MTReportParam::TYPE_STRING,    PARAMETER_CURRENCY,           DEFAULT_CURRENCY           },
      { MTReportParam::TYPE_INT,       PARAMETER_TOP_COUNT,          DEFAULT_TOP_COUNT          },
      { MTReportParam::TYPE_INT,       PARAMETER_DATA_LATENCY_MAX,   DEFAULT_DATA_LATENCY_MAX   },
     },3                                           // config parameters total
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CFtdTotalSectionReport::CFtdTotalSectionReport(void) :
   CCurrencyReport(s_info),m_client_ftd(m_currency),m_section_lead_source(m_currency),m_section_lead_campaign(m_currency),m_section_country(m_currency,true),m_section_value(m_currency)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CFtdTotalSectionReport::~CFtdTotalSectionReport(void)
  {
//--- clear common data
   Clear();
  }
//+------------------------------------------------------------------+
//| Get information about report                                     |
//+------------------------------------------------------------------+
void CFtdTotalSectionReport::Info(MTReportInfo &info)
  {
//--- get info information
   info=s_info;
  }
//+------------------------------------------------------------------+
//| Fabric method                                                    |
//+------------------------------------------------------------------+
IMTReportContext* CFtdTotalSectionReport::Create(void)
  {
   return(new (std::nothrow) CFtdTotalSectionReport());
  }
//+------------------------------------------------------------------+
//| Clear                                                            |
//+------------------------------------------------------------------+
void CFtdTotalSectionReport::Clear(void)
  {
//--- clear cache
   m_client_ftd.Clear();
//--- clear sections
   m_section_lead_source.Clear();
   m_section_lead_campaign.Clear();
   m_section_country.Clear();
   m_section_value.Clear();
  }
//+------------------------------------------------------------------+
//| Get report parameters and create caches                          |
//+------------------------------------------------------------------+
MTAPIRES CFtdTotalSectionReport::Prepare(void)
  {
//--- checks
   if(m_api==NULL)
      return(MT_RET_ERR_PARAMS);
//--- initialize sections
   MTAPIRES res=m_section_lead_source.Initialize();
   if(res!=MT_RET_OK)
      return(res);
   if((res=m_section_lead_campaign.Initialize())!=MT_RET_OK)
      return(res);
   if((res=m_section_country.Initialize())!=MT_RET_OK)
      return(res);
   if((res=m_section_value.Initialize())!=MT_RET_OK)
      return(res);
//--- initialize cache
   if((res=m_client_ftd.Initialize(*m_api,m_params))!=MT_RET_OK)
      return(res);
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Request data from server and calculate all statistics            |
//+------------------------------------------------------------------+
MTAPIRES CFtdTotalSectionReport::Calculate(void)
  {
//--- check
   if(!m_api)
      return(MT_RET_ERR_PARAMS);
//--- update cache
   MTAPIRES res=m_client_ftd.Update(m_api->ParamFrom(),m_api->ParamTo());
   if(res!=MT_RET_OK)
      return(res);
//--- read cache
   if((res=m_client_ftd.Read(this))!=MT_RET_OK)
      return(res);
//--- calculate top
   return(CalculateTop());
  }
//+------------------------------------------------------------------+
//| Client FTD read handler                                          |
//+------------------------------------------------------------------+
MTAPIRES CFtdTotalSectionReport::ClientFtdRead(const CClientCache::ClientCache &client,const INT64 time,const double value)
  {
//--- add deposit to sections
   MTAPIRES res=m_section_lead_source.DepositAdd(value,client.lead_source);
   if(res!=MT_RET_OK)
      return(res);
   if((res=m_section_lead_campaign.DepositAdd(value,client.lead_campaign))!=MT_RET_OK)
      return(res);
   if((res=m_section_country.DepositAdd(value,client.country))!=MT_RET_OK)
      return(res);
   if((res=m_section_value.DepositAdd(value,SReportMoneyGroup::GroupIndex(value)))!=MT_RET_OK)
      return(res);
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Calculate top data                                               |
//+------------------------------------------------------------------+
MTAPIRES CFtdTotalSectionReport::CalculateTop(void)
  {
//--- calculate sections top
   MTAPIRES res=m_section_lead_source.CalculateTop(m_params);
   if(res!=MT_RET_OK)
      return(res);
   if((res=m_section_lead_campaign.CalculateTop(m_params))!=MT_RET_OK)
      return(res);
   if((res=m_section_country.CalculateTop(m_params))!=MT_RET_OK)
      return(res);
   if((res=m_section_value.CalculateTop(m_params))!=MT_RET_OK)
      return(res);
//--- begin read clients
   const CReportCache *cache=nullptr;
   if((res=m_client_ftd.ClientsReadBegin(cache))!=MT_RET_OK)
      return(res);
   if(!cache)
      return(MT_RET_ERROR);
//--- initialize sections top names
   const MTAPIRES res_lead_source=m_section_lead_source.ReadTopNames(*cache,CClientCache::DICTIONARY_ID_LEAD_SOURCE);
   const MTAPIRES res_lead_campaign=m_section_lead_campaign.ReadTopNames(*cache,CClientCache::DICTIONARY_ID_LEAD_CAMPAIGN);
   const MTAPIRES res_country=m_section_country.ReadTopNames(*cache,CClientCache::DICTIONARY_ID_COUNTRY);
//--- end read clients
   res=m_client_ftd.ClientsReadEnd();
//--- check errors
   if(res_lead_source!=MT_RET_OK)
      return(res_lead_source);
   if(res_lead_campaign!=MT_RET_OK)
      return(res_lead_campaign);
   if(res_country!=MT_RET_OK)
      return(res_country);
//--- check error code
   if(res!=MT_RET_OK)
      return(res);
//--- fill top sections names by money group
   if((res=m_section_value.MoneyGroupTopNames())!=MT_RET_OK)
      return(res);
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Prepare all graphs to be shown                                   |
//+------------------------------------------------------------------+
MTAPIRES CFtdTotalSectionReport::PrepareGraphs(void)
  {
//--- double check
   if(m_api==NULL)
      return(MT_RET_ERR_PARAMS);
//--- prepare sections graphs
   MTAPIRES res=m_section_lead_source.PrepareGraphs(*m_api,L"Lead Source",0);
   if(res!=MT_RET_OK)
      return(res);
   if((res=m_section_lead_campaign.PrepareGraphs(*m_api,L"Lead Campaign",1))!=MT_RET_OK)
      return(res);
   if((res=m_section_country.PrepareGraphs(*m_api,L"Country",2))!=MT_RET_OK)
      return(res);
   if((res=m_section_value.PrepareGraphs(*m_api,L"Value",3))!=MT_RET_OK)
      return(res);
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+