//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "PluginContext.h"
//+------------------------------------------------------------------+
//| Class for formatting one Trade record                            |
//+------------------------------------------------------------------+
class CNFATradeRecord
  {
private:
   UINT64            m_trade_id;
   CMTStr64          m_order_id;
   CMTStr16          m_product_code;
   WCHAR             m_verb;
   double            m_bid_price;
   double            m_ask_price;
   UINT64            m_quantity;
   UINT64            m_remaining_qty;
   double            m_fill_price;
   double            m_commission;
   //--- digits
   UINT              m_digits;
   UINT              m_digits_currency;

public:
                     CNFATradeRecord(void);
                    ~CNFATradeRecord(void);
   //--- return false if m_order_id is empty string
   bool              Empty(void) const;
   //--- filling
   void              Fill(LPCWSTR order_id,const IMTDeal *deal);
   void              SetPartial(const IMTOrder *order,const IMTDeal *deal);
   void              SetBidAsk(const IMTRequest *request);
   void              SetBidAsk(const MTTickShort &tick);
   //--- format record to string
   LPCWSTR           Print(CMTStr &str,tm &ttm_batch,tm &ttm_stamp,const PluginContext &context) const;
  };
//+------------------------------------------------------------------+
