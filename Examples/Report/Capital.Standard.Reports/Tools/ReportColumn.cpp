//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "ReportColumn.h"
//+------------------------------------------------------------------+
//| add table widget                                                 |
//+------------------------------------------------------------------+
IMTReportDashboardWidget* ReportColumn::TableAdd(IMTReportAPI &api,MTAPIRES &res,IMTDataset *data,const LPCWSTR title,const LPCWSTR description)
  {
//--- checks
   if(!data || !title)
     {
      res=MT_RET_ERR_PARAMS;
      return(nullptr);
     }
//--- add widget
   IMTReportDashboardWidget *table=api.DashboardWidgetAppend();
   if(!table)
     {
      res=MT_RET_ERR_PARAMS;
      return(nullptr);
     }
//--- set widget type
   if((res=table->Type(IMTReportDashboardWidget::WIDGET_TYPE_TABLE))!=MT_RET_OK)
      return(nullptr);
//--- set title
   if((res=table->Title(title))!=MT_RET_OK)
      return(nullptr);
//--- set description
   if(description)
      if((res=table->Description(description))!=MT_RET_OK)
         return(nullptr);
//--- add dataset
   if((res=table->Data(data))!=MT_RET_OK)
      return(nullptr);
//--- ok
   res=MT_RET_OK;
   return(table);
  }
//+------------------------------------------------------------------+
//| add report data columns                                          |
//+------------------------------------------------------------------+
MTAPIRES ReportColumn::ReportColumnsAdd(IMTDataset &data,const ReportColumn *columns,const UINT total)
  {
//--- checks
   if(!columns || !total)
      return(MT_RET_ERR_PARAMS);
//--- create column
   IMTDatasetColumn *column=data.ColumnCreate();
   if(!column)
      return(MT_RET_ERR_MEM);
//--- iterate columns
   MTAPIRES res;
   for(UINT i=0;i<total;i++)
     {
      //--- fill report data column
      if((res=columns[i].ReportColumnFill(*column))!=MT_RET_OK)
         break;
      //--- add report data column
      if((res=data.ColumnAdd(column))!=MT_RET_OK)
         break;
     }
//--- release column
   column->Release();
//--- result
   return(res);
  }
//+------------------------------------------------------------------+
//| fill report data column                                          |
//+------------------------------------------------------------------+
MTAPIRES ReportColumn::ReportColumnFill(IMTDatasetColumn &column) const
  {
//--- clear
   MTAPIRES res;
   if((res=column.Clear())!=MT_RET_OK)
      return(res);
//--- fill info
   if((res=column.ColumnID(id))!=MT_RET_OK)
      return(res);
   if((res=column.Name(name))!=MT_RET_OK)
      return(res);
   if((res=column.Type(type))!=MT_RET_OK)
      return(res);
   if((res=column.Width(width))!=MT_RET_OK)
      return(res);
   if((res=column.WidthMax(width_max))!=MT_RET_OK)
      return(res);
   if((res=column.Offset(offset))!=MT_RET_OK)
      return(res);
   if((res=column.Size(size))!=MT_RET_OK)
      return(res);
   if(digits_column>0)
     {
      if((res=column.DigitsColumn(UINT(digits_column)))!=MT_RET_OK)
         return(res);
     }
   else
      if(digits_column<0)
        {
         if((res=column.Digits(UINT(-digits_column)))!=MT_RET_OK)
            return(res);
        }
      else
         if(type==IMTDatasetColumn::TYPE_DOUBLE)
           {
            if((res=column.Digits(2))!=MT_RET_OK)
               return(res);
           }
   if((res=column.Flags(flags))!=MT_RET_OK)
      return(res);
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
