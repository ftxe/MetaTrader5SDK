//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Report parameter class                                           |
//+------------------------------------------------------------------+
class CReportParameter
  {
private:
   IMTConReport     *m_report;                     // report configuration interface
   IMTConParam      *m_param;                      // parameter interface

public:
                     CReportParameter(void);
                    ~CReportParameter(void);
   //--- initialization
   MTAPIRES          Initialize(IMTReportAPI &api);
   //--- clear
   void              Clear(void);
   //--- get report parameter int value
   MTAPIRES          ValueInt(INT64 &value,LPCWSTR param_name,LPCWSTR value_default);
   //--- get report parameter string value
   MTAPIRES          ValueString(LPCWSTR &value,LPCWSTR param_name,LPCWSTR value_default);
private:
   //--- get report parameter
   MTAPIRES          ParamGet(LPCWSTR param_name,const UINT type,LPCWSTR value_default);
  };
//+------------------------------------------------------------------+
