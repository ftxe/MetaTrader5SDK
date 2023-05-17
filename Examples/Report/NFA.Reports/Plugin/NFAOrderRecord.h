//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "PluginContext.h"
#pragma once
//+------------------------------------------------------------------+
//| Class for formatting one Order record                            |
//+------------------------------------------------------------------+
class CNFAOrderRecord
  {
private:
   UINT              m_trans_type;
   UINT              m_order_type;
   WCHAR             m_verb;
   CMTStr64          m_order_id;
   CMTStr16          m_product_code;
   double            m_bid_price;
   double            m_ask_price;
   double            m_limit_price;
   double            m_stop_price;
   UINT              m_duration;
   INT64             m_expiry_date;
   UINT              m_origin;
   UINT64            m_quantity;
   UINT64            m_remaining_qty;
   UINT64            m_custacc_id;
   //--- digits
   UINT              m_digits;

public:
   //--- transaction type
   enum EnTransactionType
     {
      TRANS_UNDEFINED      =0,
      TRANS_ORDER_ADDED    =1,
      TRANS_ORDER_MODIFY   =2,
      TRANS_ORDER_FILL     =3,
      TRANS_PARTIAL_FILL   =4,
      TRANS_ORDER_CANCELLED=5,
      TRANS_ORDER_DELETED  =6,
      TRANS_DLR_OFFSET     =7,
      TRANS_STOP_ADJUST    =8
     };
   //--- order type
   enum EnOrderType
     {
      ORDER_MARKET         =0,
      ORDER_STOP           =1,
      ORDER_LIMIT          =2,
      ORDER_STOP_LIMIT     =3
     };
   //--- duration
   enum EnDuration
     {
      DURATION_GTC         =0,
      DURATION_GTD         =1,
      DURATION_GFD         =2,
      DURATION_FOK         =3,
      DURATION_IOC         =4
     };
   //--- origin
   enum EnOrigin
     {
      ORIGIN_AUTO          =0,
      ORIGIN_CUST          =1,
      ORIGIN_DLR           =2,
      ORIGIN_SYSTEM        =3
     };

public:
                     CNFAOrderRecord(void);
                    ~CNFAOrderRecord(void);
   //--- check empty
   bool              Empty(void) const;
   //--- access
   LPCWSTR           OrderID(void) const;
   UINT              Origin(void) const;
   //--- format record to string
   LPCWSTR           Print(CMTStr &str,const tm &ttm_batch,const tm &ttm_stamp,const PluginContext &context) const;
   //--- filling data
   void              FillMarket(const UINT trans_type,const bool instant,const IMTOrder *order,const UINT origin);
   void              FillPending(const UINT trans_type,const IMTOrder *order,const EnOrigin origin);
   void              FillStopLoss(const UINT trans_type,const UINT64 id,const double price,const UINT64 volume_ext,const double contract_size,const IMTPosition *position,const UINT origin);
   void              FillTakeProfit(const UINT trans_type,const UINT64 id,const double price,const UINT64 volume_ext,const double contract_size,const IMTPosition *position,const UINT origin);
   //--- change some fields
   void              SetTransType(const UINT trans_type);
   void              SetPartial(const IMTOrder *order,const IMTDeal *deal);
   void              SetOrderType(const EnOrderType order_type);
   void              SetBidAsk(const IMTRequest *request);
   void              SetBidAsk(const MTTickShort &tick);

private:
   UINT              ReasonToOrigin(const UINT origin,const UINT reason) const;
   //--- return Enums as string constants
   LPCWSTR           PrintTransType(void) const;
   LPCWSTR           PrintOrderType(void) const;
   LPCWSTR           PrintDuration(void) const;
   LPCWSTR           PrintOrigin(void) const;
   LPCWSTR           PrintExpiration(CMTStr &str,const int tz_minutes) const;
  };
//+------------------------------------------------------------------+
