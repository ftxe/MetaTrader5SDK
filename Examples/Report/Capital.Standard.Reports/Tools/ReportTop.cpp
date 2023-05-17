//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "ReportTop.h"
//+------------------------------------------------------------------+
//| clear                                                            |
//+------------------------------------------------------------------+
void CReportTopBase::Clear(void)
  {
   m_index.Clear();
   m_index_sorted.Clear();
  }
//+------------------------------------------------------------------+
//| get top count report parameter value                             |
//+------------------------------------------------------------------+
MTAPIRES CReportTopBase::TopCount(CReportParameter &params,UINT &top_count,LPCWSTR param_name,LPCWSTR top_count_default)
  {
//--- checks
   if(!param_name || !*param_name || !top_count_default || !*top_count_default)
      return(MT_RET_ERR_PARAMS);
//--- get top count from report parameters
   INT64 value=0;
   const MTAPIRES res=params.ValueInt(value,param_name,top_count_default);
   if(res!=MT_RET_OK)
      return(res);
//--- check top count
   if(value<0 || value>TOP_COUNT_MAX)
      return(MT_RET_ERR_PARAMS);
//--- check for empty
   top_count=(UINT)value;
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
//| sort index static method                                         |
//+------------------------------------------------------------------+
int CReportTopBase::SortIndex(const void *left,const void *right)
  {
//--- type conversion
   const UINT &l=*(const UINT*)left;
   const UINT &r=*(const UINT*)right;
//--- compare indexes
   if(l<r)
      return(-1);
   if(l>r)
      return(1);
   return(0);
  }
//+------------------------------------------------------------------+
