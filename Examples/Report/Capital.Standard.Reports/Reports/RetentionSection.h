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
#include "..\Cache\ReportCache.h"
//+------------------------------------------------------------------+
//| Retention by section Report part class                           |
//+------------------------------------------------------------------+
class CRetentionSection
  {
private:
   enum constants
     {
      STEP_MAX     =28,                               // time step count
      STEP_DURATION=SECONDS_IN_DAY                    // time step duration
     };

private:
   #pragma pack(push,1)
   //--- Retention dataset record
   struct RetentionRecord
     {
      wchar_t           day[32];                     // week
      double            sections[1];                  // retention by sections
     };
   #pragma pack(pop)
   //--- Uint structure add operation
   class CUIntOperationAdd
     {
   public:
      void              operator()(UINT &l,const UINT r) const { l+=r; }
     };
   //--- Retention container types
   typedef TReportMatrix<UINT> RetentionMatrix;       // retention matrix type
   typedef RetentionMatrix::TVector RetentionVector;  // retention vector type
   typedef TReportTop<UINT> RetentionTop;             // retention top type

private:
   RetentionMatrix   m_retention;                     // retention matrix
   RetentionTop      m_top;                           // top section indexes
   CReportDictionary m_names;                         // section names
   //--- static data
   static ReportColumn s_columns_retention[];         // column descriptions retention

public:
   //--- constructor/destructor
                     CRetentionSection(void);
                    ~CRetentionSection(void);
   //--- clear
   void              Clear(void);
   //--- Initialize
   MTAPIRES          Initialize(void);
   //--- add lifetime
   MTAPIRES          LifetimeAdd(const INT64 lifetime,const UINT section);
   //--- calculate total
   MTAPIRES          CalculateTotal(CReportParameter &params);
   //--- calculate period sections
   MTAPIRES          CalculatePeriod(const CReportInterval &interval);
   //--- read top sections names
   MTAPIRES          ReadTopNames(const CReportCache &cache,const UINT dictionary_id);
   //--- prepare all graphs to be shown
   MTAPIRES          PrepareGraphs(IMTReportAPI &api,LPCWSTR section_type,LPCWSTR description);

private:
   //--- retention by time and section
   UINT*             RetentionByTimeAndSection(const INT64 lifetime,const UINT section);
   //--- fill period top names
   static MTAPIRES   FillPeriodTopNames(const CReportInterval &interval,const CReportTopBase &top,CReportDictionary &names);
   //--- prepare retention data
   MTAPIRES          PrepareData(IMTDataset &data) const;
   //--- prepare retention data columns
   MTAPIRES          PrepareDataColumns(IMTDataset &data) const;
   //--- fill record
   bool              FillRecord(RetentionRecord &row,const RetentionVector &sections,const RetentionVector &first,const UINT other_first) const;
   //--- prepare graph
   MTAPIRES          PrepareGraph(IMTReportAPI &api,IMTDataset *data,LPCWSTR title,LPCWSTR description,const UINT type,const UINT column_id);
   //--- sort retention descending
   static int        SortRetentionDesc(const void *left,const void *right);
  };
//+------------------------------------------------------------------+
