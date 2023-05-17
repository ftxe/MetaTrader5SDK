//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "..\Tools\ReportColumn.h"
#include "..\Tools\ReportTop.h"
#include "..\Tools\ReportDictionary.h"
#include "..\Tools\CurrencyConverter.h"
#include "..\Tools\ReportMoneySection.h"
#include "..\Tools\ReportMoneyGroup.h"
#include "..\Cache\ReportCache.h"
//+------------------------------------------------------------------+
//| FTD total by section Report part class                           |
//+------------------------------------------------------------------+
class CFtdTotalSection
  {
private:
   #pragma pack(push,1)
   //--- FTD section record
   struct FtdSectionRecord
     {
      wchar_t           section[64];               // section
      double            value;                     // ftd
      double            amount;                    // amount
      UINT64            count;                     // count
     };
   #pragma pack(pop)
   //--- FTD container types
   typedef ReportMoney Ftd;                        // FTD type
   typedef TReportVector<Ftd> FtdVector;           // FTD vector type
   typedef TReportTop<Ftd> FtdTop;                 // FTD top type

private:
   CCurrencyConverter &m_currency;                 // currency converter reference
   const bool        m_geo;                        // geo section flag
   FtdVector         m_total;                      // FTD total vector
   FtdTop            m_top;                        // top section indexes
   CReportDictionary m_names;                      // section names
   //--- static data
   static ReportColumn s_columns_ftd[];            // column descriptions FTD

public:
   //--- constructor/destructor
   explicit          CFtdTotalSection(CCurrencyConverter &currency,const bool geo=false);
                    ~CFtdTotalSection(void);
   //--- clear
   void              Clear(void);
   //--- Initialize
   MTAPIRES          Initialize(void);
   //--- add deposit
   MTAPIRES          DepositAdd(const double value,const UINT section);
   //--- calculate top
   MTAPIRES          CalculateTop(CReportParameter &params);
   //--- read top sections names
   MTAPIRES          ReadTopNames(const CReportCache &cache,const UINT dictionary_id);
   //--- fill top sections names by money group
   MTAPIRES          MoneyGroupTopNames(void);
   //--- prepare all graphs to be shown
   MTAPIRES          PrepareGraphs(IMTReportAPI &api,LPCWSTR section_type,const UINT y,const bool amount_only=false);

private:
   //--- prepare FTD data
   MTAPIRES          PrepareData(IMTDataset &data) const;
   //--- fill record section
   void              FillRecordSection(FtdSectionRecord &record,const Ftd &ftd) const;
   //--- prepare graph
   MTAPIRES          PrepareGraph(IMTReportAPI &api,IMTDataset *data,LPCWSTR title,const UINT column_id,const UINT x,const UINT y);
  };
//+------------------------------------------------------------------+
