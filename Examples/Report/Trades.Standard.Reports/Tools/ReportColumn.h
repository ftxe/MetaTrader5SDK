//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Macros for calc member size                                      |
//+------------------------------------------------------------------+
#define MtFieldSize(type,member) (sizeof(((type*)(0))->member))
//+------------------------------------------------------------------+
//| Report column description (description of record field)          |
//+------------------------------------------------------------------+
struct ReportColumn
  {
   UINT              id;               // unique column id (must be greater than 0)
   LPCWSTR           name;             // visible column title
   UINT              type;             // data type and formating of column
   UINT              width;            // relative width
   UINT              width_max;        // max width in pixel
   UINT              offset;           // field offset
   UINT              size;             // size in bytes for strings
   UINT              digits_column;    // id of column with digits value
   UINT64            flags;            // flags
   //--- add table widget
   static IMTReportDashboardWidget* TableAdd(IMTReportAPI &api,MTAPIRES &res,IMTDataset *data,LPCWSTR title);
   //--- create dataset from array
   static IMTDataset* DatasetFromArray(IMTReportAPI &api,MTAPIRES &res,const ReportColumn *columns,UINT columns_total,CMTArrayBase &arr);
   //--- add report data columns
   static MTAPIRES   ReportColumnsAdd(IMTDataset &data,const ReportColumn *columns,UINT columns_total);
   //--- prepare report table
   static MTAPIRES   ReportTablePrepare(IMTReportAPI &api,const ReportColumn *columns,UINT columns_total);
   //--- fill report data column
   MTAPIRES          ReportColumnFill(IMTDatasetColumn &column) const;
  };
//+------------------------------------------------------------------+
