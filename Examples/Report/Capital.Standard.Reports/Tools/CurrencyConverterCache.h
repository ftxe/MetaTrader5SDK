//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "CurrencyConverter.h"
//+------------------------------------------------------------------+
//| Currency converter class with cache                              |
//+------------------------------------------------------------------+
class CCurrencyConverterCache : public CCurrencyConverter
  {
private:
   typedef TMTArray<double,0x100> DoubleArray;     // doubles array type

private:
   const IMTReportCache *m_cache;                  // cache pointer
   UINT              m_dictionary_id;              // currency dictionary id
   UINT              m_currency_pos;               // currency dictionary position
   DoubleArray       m_rates;                      // currency rates array

public:
                     CCurrencyConverterCache(void);
                    ~CCurrencyConverterCache(void);
   //--- initialization
   MTAPIRES          Initialize(IMTReportAPI *api,CReportParameter &params,const IMTReportCache *cache,const UINT dictionary_id,LPCWSTR param_name=PARAMETER_CURRENCY,LPCWSTR currency_default=DEFAULT_CURRENCY);
   //--- clear
   void              Clear(void);
   //--- check currency for conversion needed
   bool              NeedConversion(const UINT currency);
   //--- currency conversion rate
   MTAPIRES          CurrencyRate(double &rate,const UINT currency);
  };
//+------------------------------------------------------------------+
