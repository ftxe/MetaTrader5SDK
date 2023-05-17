//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "DatasetField.h"
//+------------------------------------------------------------------+
//| constructor                                                      |
//+------------------------------------------------------------------+
CDatasetRequest::CDatasetRequest(const DatasetField *fields,const UINT fields_total,IMTDatasetRequest &request) :
   m_fields(fields),m_fields_total(fields_total),m_request(request),m_field(nullptr),m_reference(nullptr),m_pos(UINT_MAX)
  {
  }
//+------------------------------------------------------------------+
//| destructor                                                       |
//+------------------------------------------------------------------+
CDatasetRequest::~CDatasetRequest(void)
  {
//--- release fields
   if(m_field)
     {
      m_field->Release();
      m_field=nullptr;
     }
   if(m_reference)
     {
      m_reference->Release();
      m_reference=nullptr;
     }
  }
//+------------------------------------------------------------------+
//| next field                                                       |
//+------------------------------------------------------------------+
UINT CDatasetRequest::Next(const MTAPIRES res)
  {
//--- check result
   if(res!=MT_RET_OK)
      return(0);
//--- first or next field
   if(m_pos==UINT_MAX)
      m_pos=0;
   else
      m_pos++;
//--- check position and return field id
   return(m_fields && m_pos<m_fields_total ? m_fields[m_pos].id : 0);
  }
//+------------------------------------------------------------------+
//| add field to dataset request                                     |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldAdd(void)
  {
//--- fill field
   MTAPIRES res=FieldFill();
   if(res!=MT_RET_OK)
      return(res);
//--- add field to request
   return(m_request.FieldAdd(m_field));
  }
//+------------------------------------------------------------------+
//| add field to dataset request with where UINT condition           |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldAddWhereUInt(const UINT value)
  {
//--- fill field
   MTAPIRES res=FieldFill();
   if(res!=MT_RET_OK)
      return(res);
//--- add where UINT condition
   if((res=m_field->WhereAddUInt(value))!=MT_RET_OK)
      return(res);
//--- add field to request
   return(m_request.FieldAdd(m_field));
  }
//+------------------------------------------------------------------+
//| add field to dataset request with where UINT array condition     |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldAddWhereUIntArray(const UINT64 *values,const UINT total)
  {
//--- check parameters
   if(!values || !total)
      return(MT_RET_ERR_PARAMS);
//--- fill field
   MTAPIRES res=FieldFill();
   if(res!=MT_RET_OK)
      return(res);
//--- add where condition
   if((res=m_field->WhereAddUIntArray(values,total))!=MT_RET_OK)
      return(res);
//--- add field to request
   return(m_request.FieldAdd(m_field));
  }
//+------------------------------------------------------------------+
//| add field to dataset request with where keys condition           |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldAddWhereKeys(const IMTReportCacheKeySet &keyset)
  {
//--- check parameters
   if(!keyset.Total())
      return(MT_RET_ERR_PARAMS);
//--- fill field
   MTAPIRES res=FieldFill();
   if(res!=MT_RET_OK)
      return(res);
//--- set where condition
   if((res=m_field->WhereUIntSet(keyset.Array(),keyset.Total()))!=MT_RET_OK)
      return(res);
//--- add field to request
   return(m_request.FieldAdd(m_field));
  }
//+------------------------------------------------------------------+
//| add field to dataset request with where string condition         |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldAddWhereString(LPCWSTR value)
  {
//--- check
   if(!value)
      return(MT_RET_ERR_PARAMS);
//--- fill field
   MTAPIRES res=FieldFill();
   if(res!=MT_RET_OK)
      return(res);
//--- add where string condition
   if((res=m_field->WhereAddString(value))!=MT_RET_OK)
      return(res);
//--- add field to request
   return(m_request.FieldAdd(m_field));
  }
//+------------------------------------------------------------------+
//| add field to dataset request with where string param condition   |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldAddWhereStringParam(const IMTConParam *param,LPCWSTR value_ignore)
  {
//--- checks
   if(!param)
      return(MT_RET_ERR_PARAMS);
//--- param value
   LPCWSTR value=param->Value();
   if(!value)
      return(MT_RET_ERR_PARAMS);
//--- check value
   if(value_ignore && !CMTStr::Compare(value,value_ignore))
      return(MT_RET_OK);
//--- add field to dataset request with where string condition
   return(FieldAddWhereString(value));
  }
//+------------------------------------------------------------------+
//| add field to dataset request with between int condition          |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldAddBetweenInt(const INT64 from,const INT64 to)
  {
//--- check
   if(from>to)
      return(MT_RET_ERR_PARAMS);
//--- fill field
   MTAPIRES res=FieldFill();
   if(res!=MT_RET_OK)
      return(res);
//--- add where int condition
   if((res=m_field->BetweenInt(from,to))!=MT_RET_OK)
      return(res);
//--- add field to request
   return(m_request.FieldAdd(m_field));
  }
//+------------------------------------------------------------------+
//| add field to dataset request with between uint condition         |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldAddBetweenUInt(const UINT64 from,const UINT64 to)
  {
//--- check
   if(from>to)
      return(MT_RET_ERR_PARAMS);
//--- fill field
   MTAPIRES res=FieldFill();
   if(res!=MT_RET_OK)
      return(res);
//--- add where uint condition
   if((res=m_field->BetweenUInt(from,to))!=MT_RET_OK)
      return(res);
//--- add field to request
   return(m_request.FieldAdd(m_field));
  }
//+------------------------------------------------------------------+
//| add field to dataset request with between double condition       |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldAddBetweenDouble(const double from,const double to)
  {
//--- check
   if(from>to)
      return(MT_RET_ERR_PARAMS);
//--- fill field
   MTAPIRES res=FieldFill();
   if(res!=MT_RET_OK)
      return(res);
//--- add where double condition
   if((res=m_field->BetweenDouble(from,to))!=MT_RET_OK)
      return(res);
//--- add field to request
   return(m_request.FieldAdd(m_field));
  }
//+------------------------------------------------------------------+
//| update field of dataset request with between int condition       |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldSetBetweenInt(const INT64 from,const INT64 to)
  {
//--- check
   if(from>to)
      return(MT_RET_ERR_PARAMS);
//--- get field reference from request
   MTAPIRES res=FieldReference();
   if(res!=MT_RET_OK)
      return(res);
//--- set between int condition
   return(m_reference->BetweenInt(from,to));
  }
//+------------------------------------------------------------------+
//| update field of dataset request with between uint condition      |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldSetBetweenUInt(const UINT64 from,const UINT64 to)
  {
//--- check
   if(from>to)
      return(MT_RET_ERR_PARAMS);
//--- get field reference from request
   MTAPIRES res=FieldReference();
   if(res!=MT_RET_OK)
      return(res);
//--- set between uint condition
   return(m_reference->BetweenUInt(from,to));
  }
//+------------------------------------------------------------------+
//| fill current field                                               |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldFill(void)
  {
//--- check field position
   if(!m_fields || m_pos>=m_fields_total)
      return(MT_RET_ERROR);
//--- check field
   if(!m_field)
     {
      //--- create field
      m_field=m_request.FieldCreate();
      if(!m_field)
         return(MT_RET_ERR_MEM);
     }
//--- fill field
   return(m_fields[m_pos].FieldFill(*m_field));
  }
//+------------------------------------------------------------------+
//| get field reference from request                                 |
//+------------------------------------------------------------------+
MTAPIRES CDatasetRequest::FieldReference(void)
  {
//--- check field position
   if(m_pos>=m_fields_total)
      return(MT_RET_ERR_PARAMS);
//--- check field
   if(m_reference)
      m_reference->Release();
//--- create field reference
   m_reference=m_request.FieldCreateReference(m_pos);
   return(m_reference ? MT_RET_OK : MT_RET_ERR_NOTFOUND);
  }
//+------------------------------------------------------------------+
//| fill dataset request field                                       |
//+------------------------------------------------------------------+
MTAPIRES DatasetField::FieldFill(IMTDatasetField &field) const
  {
//--- clear
   MTAPIRES res;
   if((res=field.Clear())!=MT_RET_OK)
      return(res);
//--- fill info
   if((res=field.Id(id))!=MT_RET_OK)
      return(res);
   if((res=field.Offset(offset))!=MT_RET_OK)
      return(res);
   if((res=field.Size(size))!=MT_RET_OK)
      return(res);
   if((res=field.Flags(select ? IMTDatasetField::FLAG_SELECT : 0))!=MT_RET_OK)
      return(res);
//--- ok
   return(MT_RET_OK);
  }
//+------------------------------------------------------------------+
