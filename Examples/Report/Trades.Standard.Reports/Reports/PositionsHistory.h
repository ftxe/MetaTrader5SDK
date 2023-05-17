//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "..\Tools\ReportGroup.h"
#include "..\Tools\HashTable.h"
#include "..\Tools\DatasetField.h"
#include "..\Tools\BaseSelect.h"
#include "..\Tools\HistorySelect.h"
#include "..\Tools\GroupCache.h"
//+------------------------------------------------------------------+
//| Daily Postions Report                                            |
//+------------------------------------------------------------------+
class CPositionsHistory : public CReportGroup
  {
private:
   //--- column ids
   enum columns
     {
      COLUMN_LOGIN          =1,
      COLUMN_NAME           =2,
      COLUMN_OPEN_TIME      =3,
      COLUMN_POSITION       =4,
      COLUMN_TYPE           =5,
      COLUMN_VOLUME         =6,
      COLUMN_SYMBOL         =7,
      COLUMN_OPEN_PRICE     =8,
      COLUMN_SL             =9,
      COLUMN_TP             =10,
      COLUMN_CLOSE_TIME     =11,
      COLUMN_CLOSE_PRICE    =12,
      COLUMN_REASON         =13,
      COLUMN_COMMISSION     =14,
      COLUMN_FEE            =15,
      COLUMN_SWAP           =16,
      COLUMN_PROFIT         =17,
      COLUMN_CURRENCY       =18,
      COLUMN_COMMENT        =19,
      COLUMN_CLOSE_VOLUME   =20,
      COLUMN_DIGITS         =21,
      COLUMN_DIGITS_CURRENCY=22,
     };
   //+------------------------------------------------------------------+
   //| deal position record                                             |
   //+------------------------------------------------------------------+
   #pragma pack(push,1)
   struct DealPositionRecord
     {
      UINT64            login;               // owner client login
      UINT64            position;            // position ticket
     };
   //+------------------------------------------------------------------+
   //| deal record                                                      |
   //+------------------------------------------------------------------+
   struct DealRecord
     {
      UINT64            login;               // owner client login
      UINT64            deal;                // deal ticket
      UINT64            position_id;         // deal position ticket
      INT64             time_create;         // deal create time
      UINT              entry;               // deal entry - EnDealEntry
      UINT              action;              // deal action - EnDealAction
      wchar_t           symbol[32];          // deal symbol
      UINT64            volume;              // deal volume
      double            price_open;          // deal price
      double            sl;                  // deal SL price
      double            tp;                  // deal TP price
      double            commission;          // commission
      double            commission_fee;      // fee
      double            storage;             // storage
      double            profit;              // profit
      UINT              reason;              // deal reason - EnDealReason
      wchar_t           comment[32];         // comment
      UINT              digits;              // number of digits of deal symbol
      UINT              digits_currency;     // number of digits of deal currency
      //--- check in/out
      bool              IsOut(void) const      { return(entry==IMTDeal::ENTRY_OUT); }
      bool              IsInOut(void) const    { return(entry==IMTDeal::ENTRY_INOUT); }
      //--- check type
      bool              IsBuy (void) const     { return(action==IMTDeal::DEAL_BUY); }
      bool              IsSell(void) const     { return(action==IMTDeal::DEAL_SELL); }
      //--- check balance operation
      bool              IsBalance(void) const { return(IsBalance(action)); }
      static bool       IsBalance(const UINT action);
      //--- check service deal
      inline bool       IsService(void) const;
     };
   //+------------------------------------------------------------------+
   //| order record                                                     |
   //+------------------------------------------------------------------+
   struct OrderRecord
     {
      UINT64            order;               // order ticket
      INT64             time_setup;          // time of order reception from a client into the system
     };
   //+------------------------------------------------------------------+
   //| position record                                                  |
   //+------------------------------------------------------------------+
   struct PositionRecord
     {
      UINT64            position_id{};       // position ticket
      wchar_t           symbol[32]{};        // position symbol
      UINT              type{};              // EnPositionAction
      UINT              open_reason{};
      UINT64            open_volume{};
      INT64             open_time{};
      double            open_price{};
      double            sl{};
      double            tp{};
      UINT64            close_volume{};
      INT64             close_time{};
      double            close_price{};
      double            commission{};
      double            commission_fee{};
      double            storage{};
      double            profit{};
      wchar_t           comment[32]{};       // comment
      UINT              digits{};            // number of digits of position symbol
      UINT              digits_currency{};   // number of digits of position currency

                        PositionRecord(void)=default;
                        PositionRecord(const UINT64 id) : position_id{id} {}
      //--- check balance operation
      bool              IsBalance(void) const { return(DealRecord::IsBalance(type)); }
      //--- init position history by deal
      inline bool       DealInit(const DealRecord &deal);
      //--- add deal to position history
      inline void       DealAdd(const DealRecord &deal);
     };
   //--- table record
   struct TableRecord : PositionRecord
     {
      UINT64            login{};             // login
      wchar_t           name[32]{};          // user name
      wchar_t           currency[32]{};      // currency

                        TableRecord(void)=default;
                        TableRecord(const UINT64 id) : PositionRecord{id} {}
     };
   //--- user record
   struct UserRecord
     {
      UINT64            login;               // login
      wchar_t           name[32];            // name
      wchar_t           group[64];           // group
     };
   //--- table item
   struct TableItem
     {
      UINT64            position_id{};       // position id
      UINT              deals{};             // deals count
      TableRecord      *record{};            // table record pointer

                        TableItem(const UINT64 id) : position_id{id} {}
     };
   #pragma pack(pop)
   //--- table record array type
   typedef TMTArray<TableRecord> TableRecordArray;
   //--- positions writer class
   struct            CPositionWriter;
   //--- hash functor
   struct TableItemHash
     {
      UINT64            operator()(const TableItem &item) const   { return(operator()(item.position_id)); }
      UINT64            operator()(const UINT64 id) const         { return(id); }
     };
   //--- comparsion functor
   struct TableItemEqual
     {
      bool              operator()(const TableItem &l,const TableItem &r) const  { return(operator()(l,r.position_id)); }
      bool              operator()(const TableItem &l,const UINT64 id) const     { return(l.position_id==id); }
     };
   //--- hash table type
   typedef CHashTable<TableItem,TableItemHash,TableItemEqual> TableRecords;
   typedef CHashAllocator<sizeof(TableRecord)> TableRecordAllocator;

private:
   INT64             m_from;                 // report time interval begin
   INT64             m_to;                   // report time interval end
   INT64             m_from_orders;          // earliest order time
   //--- API interfaces
   IMTReportCacheKeySet *m_deal_logins;      // deal logins
   IMTReportCacheKeySet *m_position_ids;     // position ids
   //--- select objects
   CUserSelect       m_user_select;          // user select object
   CDealSelect       m_deal_select;          // deal history select object
   CHistorySelect    m_order_select;         // order history select object
   //--- records
   TableRecord       m_record;               // current table record
   UserRecord        m_user;                 // current user record
   TableRecords      m_positions;            // history positions
   TableRecordArray  m_summaries;            // summaries
   TableRecordAllocator m_allocator;         // positions allocator
   //---
   static const MTReportInfo s_info;                  // report info
   static const ReportColumn s_columns[];             // column descriptions   
   static const DatasetField s_user_fields[];         // user request fields descriptions
   static const UINT s_user_request_limit=100000;     // user request limit
   static const DatasetField s_order_time_fields[];   // order time request fields descriptions
   static const UINT s_order_request_limit=16384;     // order request limit
   static const DatasetField s_deal_position_fields[];// deal position request fields descriptions
   static const DatasetField s_deal_fields[];         // deal request fields descriptions
   static const UINT s_deal_request_limit=16384;      // deal request limit

public:
   //--- constructor/destructor
                     CPositionsHistory(void);
   virtual          ~CPositionsHistory(void);
   //--- report information
   static void       Info(MTReportInfo& info);

private:
   //--- base overrides
   virtual void      Clear(void) override;
   virtual MTAPIRES  Prepare(void) override;
   virtual MTAPIRES  Write(void) override;
   //--- prepare requests
   MTAPIRES          PrepareDealRequest(const DatasetField *fields,UINT fields_total);
   MTAPIRES          PrepareOrderRequest(void);
   //--- collect deals logins and positions ids sets
   MTAPIRES          CollectLoginsAndPositionIds(void);
   //--- find earliest order time
   MTAPIRES          FindEarliestOrderTime(void);
   //--- write report parts
   MTAPIRES          WriteLogins(void);
   MTAPIRES          WritePositions(IMTDataset &dataset,IMTReportCacheKeySet &logins,CGroupCache &groups,CPositionWriter &writer);
   //--- read from dataset
   MTAPIRES          ReadUser(const UINT64 login,IMTDataset &dataset);
   //--- add record to positions
   MTAPIRES          PositionsAdd(const DealRecord &deal,CPositionWriter &writer);
   //--- initialize table record
   bool              TableRecordInit(TableRecord &record,const DealRecord &deal,const bool balance) const;
   //--- summary
   void              SummaryAdd(TableRecord &summary,const PositionRecord &position);
   MTAPIRES          SummaryWrite(void);
   static int        SortSummary(const void *left,const void *right);
  };
//+------------------------------------------------------------------+

