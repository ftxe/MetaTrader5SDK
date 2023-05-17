//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Macros for member size calculation                               |
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
   UINT              digits;           // id of column with digits value
   UINT64            flags;            // flags
   //--- add report data columns
   static MTAPIRES   ReportColumnsAdd(IMTReportAPI *api,const ReportColumn *columns,const UINT total);
   //--- fill report data column
   MTAPIRES          ReportColumnFill(IMTDatasetColumn &column) const;
  };
//+------------------------------------------------------------------+
