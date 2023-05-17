//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "DepositWithdrawal.h"
//+------------------------------------------------------------------+
//| Plug-in description structure                                    |
//+------------------------------------------------------------------+
MTReportInfo CDepositWithdrawal::s_info=
  {
   100,
   MTReportAPIVersion,
   0,
   L"Deposit and Withdrawal",
   L"Copyright 2000-2023, MetaQuotes Ltd.",
   L"MetaTrader 5 Report API plug-in",
   MTReportInfo::SNAPSHOT_NONE,
   MTReportInfo::TYPE_TABLE,
   L"Trades",
                   // params
     {{ MTReportParam::TYPE_GROUPS, MTAPI_PARAM_GROUPS, L"*" },
      { MTReportParam::TYPE_DATE,   MTAPI_PARAM_FROM         },
      { MTReportParam::TYPE_DATE,   MTAPI_PARAM_TO           },
     },3            // params_total
  };
//+------------------------------------------------------------------+
//| Column descriptions                                              |
//+------------------------------------------------------------------+
ReportColumn CDepositWithdrawal::s_columns[]=
  {
   //--- id,                name,             type,                        width, width_max, offset,                          size,                              digits_column,          flags
     { COLUMN_DEAL,           L"Deal",          IMTDatasetColumn::TYPE_UINT64,    10, 0,   offsetof(TableRecord,deal),            0,                                 0,                      IMTDatasetColumn::FLAG_PRIMARY },
   { COLUMN_LOGIN,          L"Login",         IMTDatasetColumn::TYPE_USER_LOGIN,10, 0,   offsetof(TableRecord,login),           0,                                 0,                      0                             },
   { COLUMN_NAME,           L"Name",          IMTDatasetColumn::TYPE_STRING,    20, 0,   offsetof(TableRecord,name),            MtFieldSize(TableRecord,name),     0,                      0                             },
   { COLUMN_TIME,           L"Time",          IMTDatasetColumn::TYPE_DATETIME,  15, 150, offsetof(TableRecord,date),            0,                                 0,                      0                             },
   { COLUMN_COMMENT,        L"Comment",       IMTDatasetColumn::TYPE_STRING,    30, 0,   offsetof(TableRecord,comment),         MtFieldSize(TableRecord,comment),  0,                      0                             },
   { COLUMN_AMOUNT,         L"Amount",        IMTDatasetColumn::TYPE_MONEY,     10, 0,   offsetof(TableRecord,amount),          0,                                 COLUMN_CURRENCY_DIGITS, 0                             },
   { COLUMN_CURRENCY,       L"Currency",      IMTDatasetColumn::TYPE_STRING,    9,  70,  offsetof(TableRecord,currency),        MtFieldSize(TableRecord,currency), 0,                      0                             },
   { COLUMN_CURRENCY_DIGITS,L"CurrencyDigits",IMTDatasetColumn::TYPE_UINT32,    0,  0,   offsetof(TableRecord,currency_digits), 0,                                 0,                      IMTDatasetColumn::FLAG_HIDDEN  }
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CDepositWithdrawal::CDepositWithdrawal(void) : m_api(NULL),m_deals(NULL),
                                               m_user(NULL),m_group(NULL)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CDepositWithdrawal::~CDepositWithdrawal(void)
  {
   Clear();
  }
//+------------------------------------------------------------------+
//| Clear                                                            |
//+------------------------------------------------------------------+
void CDepositWithdrawal::Clear(void)
  {
//--- group config interface
   if(m_group)
     {
      m_group->Release();
      m_group=NULL;
     }
//--- user interface
   if(m_user)
     {
      m_user->Release();
      m_user=NULL;
     }
//--- deals interface
   if(m_deals)
     {
      m_deals->Release();
      m_deals=NULL;
     }
//--- summaries
   m_summaries.Clear();
//--- API interface
   m_api=NULL;
  }
//+------------------------------------------------------------------+
//| Get information about report                                     |
//+------------------------------------------------------------------+
void CDepositWithdrawal::Info(MTReportInfo& info)
  {
//--- get info information  
   info=s_info;
  }
//+------------------------------------------------------------------+
//| Release plug-in                                                  |
//+------------------------------------------------------------------+
void CDepositWithdrawal::Release(void)
  {
   delete this;
  }
//+------------------------------------------------------------------+
//| Report generation                                                |
//+------------------------------------------------------------------+
MTAPIRES CDepositWithdrawal::Generate(const UINT type,IMTReportAPI *api)
  {
   UINT64  *logins=NULL;
   UINT     logins_total=0;
   MTAPIRES res;
//--- clear
   Clear();
//--- checks
   if(!api)
      return(MT_RET_ERR_PARAMS);
//--- checks
   if(type!=MTReportInfo::TYPE_TABLE)
      return(MT_RET_ERR_NOTIMPLEMENT);
//--- save api pointer
   m_api=api;
//--- prepare table
   if((res=TablePrepare())!=MT_RET_OK)
     {
      Clear();
      return(res);
     }
//--- get report logins
   if((res=api->ParamLogins(logins,logins_total))!=MT_RET_OK)
     {
      Clear();
      //--- return result
      return(res);
     }
//--- checks logins
   if(logins && logins_total)
     {
      //--- prepare deals array & user & group
      if((m_deals=api->DealCreateArray())==NULL ||
         (m_user =api->UserCreate())     ==NULL ||
         (m_group=api->GroupCreate())    ==NULL)
        {
         api->Free(logins);
         Clear();
         return(MT_RET_ERR_MEM);
        }
      //--- write table
      for(UINT i=0;i<logins_total;i++)
         if((res=TableWrite(logins[i]))!=MT_RET_OK)
           {
            api->Free(logins);
            Clear();
            return(res);
           }
     }
//--- free logins
   if(logins) api->Free(logins);
//--- write table summaries
   if((res=TableWriteSummaries())!=MT_RET_OK)
     {
      Clear();
      return(res);
     }
//--- clear 
   Clear();
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Prepare table                                                    |
//+------------------------------------------------------------------+
MTAPIRES CDepositWithdrawal::TablePrepare(void)
  {
   IMTDatasetColumn *column;
   MTAPIRES         res;
//--- checks
   if(!m_api) return(MT_RET_ERR_PARAMS);
//--- create column
   if((column=m_api->TableColumnCreate())==NULL)
      return(MT_RET_ERR_MEM);
//--- clear table columns
   m_api->TableColumnClear();
//--- add column description for TableRecord
   for(UINT i=0;i<_countof(s_columns);i++)
     {
      //--- clear
      column->Clear();
      //--- fill info
      column->ColumnID(s_columns[i].id);
      column->Name(s_columns[i].name);
      column->Type(s_columns[i].type);
      column->Width(s_columns[i].width);
      column->WidthMax(s_columns[i].width_max);
      column->Offset(s_columns[i].offset);
      column->Size(s_columns[i].size);
      column->DigitsColumn(s_columns[i].digits_column);
      column->Flags(s_columns[i].flags);
      //--- add column
      if((res=m_api->TableColumnAdd(column))!=MT_RET_OK)
        {
         column->Release();
         return(res);
        }
     }
//--- release column
   column->Release();
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Write table for specified login                                  |
//+------------------------------------------------------------------+
MTAPIRES CDepositWithdrawal::TableWrite(const UINT64 login)
  {
   IMTDeal     *deal=NULL;
   UINT         deal_total,i;
   TableRecord  record={0},summary={0},*ptr;
   bool         summary_exist=false;
   MTAPIRES     retcode;
//--- checks
   if(!login || !m_api || !m_deals) return(MT_RET_ERR_PARAMS);
//--- get deals
   if((retcode=m_api->DealGet(login,m_api->ParamFrom(),m_api->ParamTo(),m_deals))!=MT_RET_OK)
      return(retcode);
//--- nothing to do?
   if(!m_deals->Total()) return(MT_RET_OK);
//--- get user info
   if((retcode=m_api->UserGetLight(login,m_user))!=MT_RET_OK) return(retcode);
//--- get user group info
   if((retcode=m_api->GroupGetLight(m_user->Group(),m_group))!=MT_RET_OK) return(retcode);
//--- prepare summary
   CMTStr::Copy(summary.currency,m_group->Currency());
   summary.currency_digits=m_group->CurrencyDigits();
//---
   for(i=0,deal_total=m_deals->Total();i<deal_total;i++)
     {
      //--- get deal
      if((deal=m_deals->Next(i))==NULL) continue;
      //--- is balance?
      if(deal->Action()==IMTDeal::DEAL_BALANCE)
        {
         //--- clear record
         ZeroMemory(&record,sizeof(record));
         //--- fill record
         record.deal           =deal->Deal();
         record.login          =deal->Login();
         CMTStr::Copy(record.name,    m_user->Name());
         CMTStr::Copy(record.comment, deal->Comment());
         record.amount         =deal->Profit();
         record.date           =deal->Time();
         record.currency_digits=deal->DigitsCurrency();
         CMTStr::Copy(record.currency,m_group->Currency());
         //--- write row
         if((retcode=m_api->TableRowWrite(&record,sizeof(record)))!=MT_RET_OK) return(retcode);
         //--- calc summary
         summary.amount=SMTMath::MoneyAdd(summary.amount,deal->Profit(),summary.currency_digits);
         //--- exist summary
         summary_exist=true;
        }
     }
//--- check summary
   if(!summary_exist) return(MT_RET_OK);
//--- search summary
   if((ptr=m_summaries.Search(&summary,SortSummary))!=NULL)
     {
      //--- update summaries
      ptr->amount=SMTMath::MoneyAdd(ptr->amount,summary.amount,ptr->currency_digits);
     }
   else
     {
      //--- insert new summary
      if(m_summaries.Insert(&summary,SortSummary)==NULL) return(MT_RET_ERR_MEM);
     }
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Write summary                                                    |
//+------------------------------------------------------------------+
MTAPIRES CDepositWithdrawal::TableWriteSummaries(void)
  {
   IMTDatasetSummary *summary=NULL;
   UINT              i;
   MTAPIRES          res;
//--- checks
   if(!m_api) return(MT_RET_ERR_MEM);
//--- create summary
   if((summary=m_api->TableSummaryCreate())==NULL)
      return(MT_RET_ERR_MEM);
//---
   for(i=0;i<m_summaries.Total();i++)
     {
      //--- clear summary
      summary->Clear();
      //--- total text
      summary->ColumnID(COLUMN_DEAL);
      summary->MergeColumn(COLUMN_COMMENT);
      summary->Line(i);
      summary->ValueString(i==0 ? L"Total" : L"");
      //--- add summary
      if((res=m_api->TableSummaryAdd(summary))!=MT_RET_OK)
        {
         summary->Release();
         return(res);
        }
      //--- clear summary
      summary->Clear();
      //--- total amount
      summary->ColumnID(COLUMN_AMOUNT);
      summary->Line(i);
      summary->ValueMoney(m_summaries[i].amount);
      summary->Digits(m_summaries[i].currency_digits);
      //--- add summary
      if((res=m_api->TableSummaryAdd(summary))!=MT_RET_OK)
        {
         summary->Release();
         return(res);
        }
      //--- clear summary
      summary->Clear();
      //--- currency
      summary->ColumnID(COLUMN_CURRENCY);
      summary->Line(i);
      summary->ValueString(m_summaries[i].currency);
      //--- add summary
      if((res=m_api->TableSummaryAdd(summary))!=MT_RET_OK)
        {
         summary->Release();
         return(res);
        }
     }
//--- release summary
   summary->Release();
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| Sort summary                                                     |
//+------------------------------------------------------------------+
int CDepositWithdrawal::SortSummary(const void *left,const void *right)
  {
   TableRecord *lft=(TableRecord*)left;
   TableRecord *rgh=(TableRecord*)right;
//--- sort by currency
   return(CMTStr::CompareNoCase(lft->currency,rgh->currency));
  }
//+------------------------------------------------------------------+
