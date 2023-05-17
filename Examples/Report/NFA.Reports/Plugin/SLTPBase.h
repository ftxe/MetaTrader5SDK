//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "..\Tools\RecordDataBase.h"
#include "NFAOrderRecord.h"
//+------------------------------------------------------------------+
//| Record for storing SL/TP change history                          |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct SLTPRecord
  {
   UINT64            id;               // unique record id
   INT64             timestamp;        // timestamp
   UINT64            position_id;      // position id
   UINT64            sl_id;            // SL counter
   double            sl_price;         // SL price
   UINT64            tp_id;            // TP counter
   double            tp_price;         // TP price
   UINT64            volume_ext;       // volume
   INT64             time_create;      // position time create
   UINT              origin;           // order origin
   double            contract_size;    // contract size
   UINT64            reserved[7];      // reserved
   //--- read/writes
   bool              Read(CMTFile& file);
   bool              Read(CMTFile& file,const UINT version,bool &invalid_version);
   bool              Write(CMTFile& file) const;
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Permanent storage for SL/TP records                              |
//+------------------------------------------------------------------+
class CSLTPBase : protected TRecordDataBase<SLTPRecord>
  {
private:
   enum              constants { COMPACT_CHECK_START_TIME=SECONDS_IN_WEEK };

private:
   IMTServerAPI     *m_api;
   TMTArray<SLTPRecord*> m_index_position;

public:
                     CSLTPBase(void);
                    ~CSLTPBase(void);
   //--- 
   bool              Initialize(IMTServerAPI *api,const PluginContext &context);
   void              Shutdown(void);
   void              Compact(void);
   //---
   bool              RecordGet(const UINT64 position,SLTPRecord& record);
   bool              RecordUpdate(SLTPRecord& record,const bool allow_add=true);
   bool              RecordDelete(const UINT64 position);

private:
   bool              IndexRebuild(void);
   //--- logger
   virtual void      Out(const UINT code,LPCWSTR msg,...);
   //--- sorting
   static int        SortByPosition(const void *left,const void *right);
   static int        SearchByPosition(const void *left,const void *right);
  };
//+------------------------------------------------------------------+
