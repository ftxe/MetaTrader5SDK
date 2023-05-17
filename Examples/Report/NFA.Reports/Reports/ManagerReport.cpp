//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "ManagerReport.h"
#include "..\Tools\TextReader.h"
//+------------------------------------------------------------------+
//| Report description structure                                     |
//+------------------------------------------------------------------+
MTReportInfo CManagerReport::s_info=
  {
   100,
   MTReportAPIVersion,
   MTReportInfo::IE_VERSION_ANY,
   L"Manager",
   COPYRIGHT,
   L"MetaTrader 5 Report API plug-in",
   0,
   MTReportInfo::TYPE_TABLE,
   L"NFA",
     {{ MTReportParam::TYPE_DATE,MTAPI_PARAM_FROM },
      { MTReportParam::TYPE_DATE,MTAPI_PARAM_TO   },
     },
   2,   // params_total 
     {{ MTReportParam::TYPE_STRING,L"Base Directory",DEFAULT_BASE_DIRECTORY }
     },
   1    //config_total
  };
//+------------------------------------------------------------------+
//| Report fields description structures                             |
//+------------------------------------------------------------------+
ReportColumn CManagerReport::s_columns[]=
   //id                         ,name                    ,type                             ,width,width_max,offset                                    ,size                                     ,digits_column,flags
  {{ COLUMN_BATCH_DATE          ,L"BATCH_DATE"           ,IMTDatasetColumn::TYPE_DATE      ,    1,        0,offsetof(TableRecord,batch_date          ),0                                         ,0            ,0  },
   { COLUMN_MANAGER_ID          ,L"MANAGER_ID"           ,IMTDatasetColumn::TYPE_STRING    ,    1,        0,offsetof(TableRecord,manager_id          ),MtFieldSize(TableRecord,manager_id)   ,0            ,0  },
   { COLUMN_NFA_ID              ,L"NFA_ID"               ,IMTDatasetColumn::TYPE_STRING    ,    1,        0,offsetof(TableRecord,nfa_id              ),MtFieldSize(TableRecord,nfa_id           ),0            ,0  },
   { COLUMN_FDM_ID              ,L"FDM_ID"               ,IMTDatasetColumn::TYPE_STRING    ,    1,        0,offsetof(TableRecord,fdm_id              ),MtFieldSize(TableRecord,fdm_id           ),0            ,0  },
   { COLUMN_MANAGER_NAME        ,L"MANAGER_NAME"         ,IMTDatasetColumn::TYPE_STRING    ,    1,        0,offsetof(TableRecord,manager_name        ),MtFieldSize(TableRecord,manager_name     ),0            ,0  },
   { COLUMN_COUNTRY_TYPE        ,L"COUNTRY_TYPE"         ,IMTDatasetColumn::TYPE_STRING    ,    1,        0,offsetof(TableRecord,country_type        ),MtFieldSize(TableRecord,country_type     ),0            ,0  },
   { COLUMN_START_DATE          ,L"START_DATE"           ,IMTDatasetColumn::TYPE_DATETIME  ,    1,        0,offsetof(TableRecord,start_date          ),0                                         ,0            ,0  },
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CManagerReport::CManagerReport(void)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CManagerReport::~CManagerReport(void)
  {
  }
//+------------------------------------------------------------------+
//| Get report descriptor                                            |
//+------------------------------------------------------------------+
MTAPIRES CManagerReport::Info(MTReportInfo &info)
  {
   info=s_info;
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Remove report class                                              |
//+------------------------------------------------------------------+
void CManagerReport::Release(void)
  {
   delete this;
  }
//+------------------------------------------------------------------+
//| Generate report                                                  |
//+------------------------------------------------------------------+
MTAPIRES CManagerReport::Generate(const UINT type,IMTReportAPI *api)
  {
//---
   if(!api)
      return(MT_RET_ERR_PARAMS);
//--- There is only table report implemented
   if(type!=MTReportInfo::TYPE_TABLE)
      return(MT_RET_ERR_NOTIMPLEMENT);
//--- prepare columns
   MTAPIRES res;
   if((res=ReportColumn::ReportColumnsAdd(api,s_columns,_countof(s_columns)))!=MT_RET_OK)
      return(res);
//--- generate
   return(ReportContext::Generate(api,REPORT_MANAGER,Fill));
  }
//+------------------------------------------------------------------+
//|  Fill Table record from CSV line                                 |
//+------------------------------------------------------------------+
bool CManagerReport::Fill(CTextReader &reader,TableRecord &record,const ReportContext&)
  {
//--- fill table record
   reader.Date   (record.batch_date  );
   reader.String (record.manager_id  );
   reader.String (record.nfa_id      );
   reader.String (record.fdm_id      );
   reader.String (record.manager_name);
   reader.String (record.country_type);
   reader.Date   (record.start_date  );
//---
   return(true);
  }
//+------------------------------------------------------------------+
