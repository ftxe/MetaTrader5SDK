//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "NFAOrderRecord.h"
//+------------------------------------------------------------------+
//| Record for storing Request history                               |
//+------------------------------------------------------------------+
struct RequestRecord
  {
   UINT64            request_id;
   CNFAOrderRecord   record;
  };
//+------------------------------------------------------------------+
//| Memory based requests storage                                    |
//+------------------------------------------------------------------+
class CRequestBase
  {
private:
   TMTArray<RequestRecord*> m_requests;

public:
                     CRequestBase(void);
                    ~CRequestBase(void);
   //--- clear/add/remove
   void              Clear(void);
   bool              Add(const UINT64 request_id,const CNFAOrderRecord& record);
   bool              Remove(const UINT64 request_id);
   bool              Remove(const UINT64 request_id,CNFAOrderRecord& record);

private:
   //--- memory
   RequestRecord*    RequestAllocate(void);
   void              RequestFree(RequestRecord *record);
   //--- sort/search
   static int        SortRequests(const void *left,const void *right);
   static int        SearchRequests(const void *left,const void *right);
  };
//+------------------------------------------------------------------+
