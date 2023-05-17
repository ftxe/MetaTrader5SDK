//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "MoneyFlowDailyLeadCampaignReport.h"
//+------------------------------------------------------------------+
//| Plugin description structure                                     |
//+------------------------------------------------------------------+
MTReportInfo CMoneyFlowDailyLeadCampaign::s_info=
  {
   100,                                            // report version
   MTReportAPIVersion,                             // report API version
   MTReportInfo::IE_VERSION_9,                     // minimal IE version
   L"Money Flow Daily by Lead Campaign",           // report name
   L"Copyright 2000-2023, MetaQuotes Ltd.",      // copyright
   L"MetaTrader 5 Report API plug-in",             // description
   MTReportInfo::SNAPSHOT_NONE,                    // snapshot modes
   MTReportInfo::TYPE_DASHBOARD,                   // report types
   L"Capital",                                     // report category
                                                   // request parameters
     {{ MTReportParam::TYPE_GROUPS,    MTAPI_PARAM_GROUPS,  DEFAULT_GROUPS    },
      { MTReportParam::TYPE_DATE,      MTAPI_PARAM_FROM                       },
      { MTReportParam::TYPE_DATE,      MTAPI_PARAM_TO                         },
     },3,                                          // request parameters total
     {                                             // config parameters
      { MTReportParam::TYPE_STRING,    PARAMETER_CURRENCY,  DEFAULT_CURRENCY  },
      { MTReportParam::TYPE_INT,       PARAMETER_TOP_COUNT, DEFAULT_TOP_COUNT },
     },2                                           // config parameters total
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CMoneyFlowDailyLeadCampaign::CMoneyFlowDailyLeadCampaign(void) :
   CMoneyFlowDailySection(s_info,offsetof(ClientCache,lead_campaign))
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CMoneyFlowDailyLeadCampaign::~CMoneyFlowDailyLeadCampaign(void)
  {
  }
//+------------------------------------------------------------------+
//| Get information about report                                     |
//+------------------------------------------------------------------+
void CMoneyFlowDailyLeadCampaign::Info(MTReportInfo &info)
  {
//--- get info information
   info=s_info;
  }
//+------------------------------------------------------------------+
//| Fabric method                                                    |
//+------------------------------------------------------------------+
IMTReportContext* CMoneyFlowDailyLeadCampaign::Create(void)
  {
   return(new (std::nothrow) CMoneyFlowDailyLeadCampaign());
  }
//+------------------------------------------------------------------+
