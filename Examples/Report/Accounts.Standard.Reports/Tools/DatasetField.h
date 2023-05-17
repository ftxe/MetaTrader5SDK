//+------------------------------------------------------------------+
//|                           MetaTrader 5 Accounts.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Dataset request field description                                |
//+------------------------------------------------------------------+
struct DatasetField
  {
   IMTDatasetField::EnFieldId id;      // field id
   bool              select;           // select field
   UINT              offset;           // field offset in bytes
   UINT              size;             // size in bytes for strings
   //--- fill dataset request field
   MTAPIRES          FieldFill(IMTDatasetField &field) const;
   //--- clear field conditions
   static MTAPIRES   FieldClearConditions(IMTDatasetField &field,const bool select=true);
  };
//+------------------------------------------------------------------+
//| Dataset request helper class                                     |
//+------------------------------------------------------------------+
class CDatasetRequest
  {
private:
   const DatasetField *m_fields;       // fields array
   const UINT        m_fields_total;   // fields array count
   IMTDatasetRequest &m_request;       // request interface
   IMTDatasetField  *m_field;          // field interface
   UINT              m_pos;            // current field position

public:
                     CDatasetRequest(const DatasetField *fields,UINT fields_total,IMTDatasetRequest &request);
   virtual          ~CDatasetRequest(void);
   //--- next field
   UINT              Next(MTAPIRES res);
   //--- add field to dataset request
   MTAPIRES          FieldAdd(void);
   //--- add field to dataset request with where condition
   MTAPIRES          FieldAddWhereUInt(UINT value);
   MTAPIRES          FieldAddWhereUIntArray(const UINT64 *values,UINT total);
   MTAPIRES          FieldAddWhereKeys(const IMTReportCacheKeySet &keys);
   MTAPIRES          FieldAddWhereString(LPCWSTR value);
   MTAPIRES          FieldAddWhereStringParam(const IMTConParam *param,LPCWSTR value_ignore);
   //--- add field to dataset request with between condition
   MTAPIRES          FieldAddBetweenInt(INT64 from,INT64 to);
   MTAPIRES          FieldAddBetweenUInt(UINT64 from,UINT64 to);
   MTAPIRES          FieldAddBetweenDouble(double from,double to);
   //--- add field to dataset request and return reference
   IMTDatasetField*  FieldAddReference(MTAPIRES &res);

private:
   //--- fill current field
   MTAPIRES          FieldFill(void);
  };
//+------------------------------------------------------------------+
