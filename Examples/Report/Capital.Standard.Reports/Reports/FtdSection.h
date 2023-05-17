//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "..\Tools\ReportColumn.h"
#include "..\Tools\ReportMatrix.h"
#include "..\Tools\ReportTop.h"
#include "..\Tools\ReportDictionary.h"
#include "..\Tools\ReportInterval.h"
#include "..\Tools\CurrencyConverter.h"
#include "..\Tools\ReportMoneySection.h"
#include "..\Tools\ReportMoneyGroup.h"
#include "..\Cache\ReportCache.h"
//+------------------------------------------------------------------+
//| FTD by section Report part class                                 |
//+------------------------------------------------------------------+
class CFtdSection
  {
private:
   #pragma pack(push,1)
   //--- FTD section record
   struct FtdSectionRecord
     {
      double            value;                     // ftd
      double            amount;                    // amount
      UINT64            count;                     // count
     };
   //--- FTD dataset record
   struct FtdRecord
     {
      INT64             date;                      // date
      FtdSectionRecord  sections[1];               // FTD by sections
     };
   #pragma pack(pop)
   //--- FTD container types
   typedef ReportMoney Ftd;                        // FTD type
   typedef TReportMatrix<Ftd> FtdMatrix;           // FTD matrix type
   typedef FtdMatrix::TVector FtdVector;           // FTD vector type
   typedef TReportTop<Ftd> FtdTop;                 // FTD top type

private:
   CCurrencyConverter &m_currency;                 // currency converter reference
   const CReportInterval &m_interval;              // report interval reference
   FtdMatrix         m_ftd;                        // FTD matrix
   FtdVector         m_total;                      // FTD total vector
   FtdTop            m_top;                        // top section indexes
   CReportDictionary m_names;                      // section names
   //--- static data
   static ReportColumn s_columns_ftd[];            // column descriptions FTD

public:
   //--- constructor/destructor
                     CFtdSection(CCurrencyConverter &currency,const CReportInterval &interval);
                    ~CFtdSection(void);
   //--- clear
   void              Clear(void);
   //--- Initialize
   MTAPIRES          Initialize(void);
   //--- add deposit
   MTAPIRES          DepositAdd(const INT64 time,const double value,const UINT section);
   //--- calculate total
   MTAPIRES          CalculateTotal(CReportParameter &params);
   //--- read top sections names
   MTAPIRES          ReadTopNames(const CReportCache &cache,const UINT dictionary_id);
   //--- fill top sections names by money group
   MTAPIRES          MoneyGroupTopNames(void);
   //--- prepare all graphs to be shown
   MTAPIRES          PrepareGraphs(IMTReportAPI &api,LPCWSTR section_type,const int y=-1);

private:
   //--- Ftd by time and section
   Ftd*              FtdByTimeAndSection(const INT64 time,const UINT section);
   //--- prepare FTD data
   MTAPIRES          PrepareData(IMTDataset &data) const;
   //--- prepare FTD data columns
   MTAPIRES          PrepareDataColumns(IMTDataset &data) const;
   //--- fill record
   bool              FillRecord(FtdRecord &row,const FtdVector &sections) const;
   //--- fill record section
   void              FillRecordSection(FtdSectionRecord &record,const Ftd *ftd) const;
   //--- prepare graph
   MTAPIRES          PrepareGraph(IMTReportAPI &api,IMTDataset *data,LPCWSTR title,const UINT column_id,const int y);
  };
//+------------------------------------------------------------------+
