//+------------------------------------------------------------------+
//|                                     MetaTrader 5 Sample Exchange |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Class of trade instrument                                        |
//+------------------------------------------------------------------+
class CTradeInstrument
  {

protected:
   //--- structure for storing an order waiting for execution
   struct ExchangePendingOrder
     {
      //--- order
      ExchangeOrder     order;
      //--- order add time
      INT64             order_add_time;
     };
   //--- type: array of orders waiting for execution
   typedef TMTArray<ExchangePendingOrder> ExchangePendingOrdersArray;

protected:
   //--- trade instrument description
   ExchangeSymbol    m_symbol;
   //--- positions
   ExchangePositionsArray m_positions;
   //--- positions
   CMTSync           m_positions_sync;

private:
   //--- preliminary queue of orders for processing
   ExchangePendingOrdersArray m_orders_income;
   //--- queue of orders for processing
   ExchangePendingOrdersArray m_orders;
   //--- object for synchronizing an entry to the orders array
   CMTSync           m_orders_sync;
   //--- performance frequency
   static INT64      s_performance_frequency;

public:
                     CTradeInstrument();
   virtual          ~CTradeInstrument();
   //--- initialize trade instrument
   virtual void      Initialize(LPCWSTR symbol_name,
                                LPCWSTR curr_base,
                                LPCWSTR curr_profit,
                                UINT exec_mode,
                                UINT digits,
                                double settlement_price);
   //--- data processing
   bool              ProcessData(CTradeAccounts &accounts,CExchangeContext &context);
   //--- add a new order to a queue for processing
   bool              OrderAdd(const ExchangeOrder &order);
   //--- manage trade instrument description
   virtual bool      SymbolSend(CExchangeContext &context,UINT index);
   LPCWSTR           SymbolGet() const;
   //--- get all orders for login
   virtual bool      OrdersGet(UINT64 login,ExchangeOrdersArray &orders);
   //--- get position for login
   virtual bool      PositionGet(UINT64 login,ExchangePosition &position);
   virtual ExchangePosition* PositionGetPtr(UINT64 login);
   //--- get all positions
   virtual bool      PositionsGetAll(ExchangePositionsArray &positions);

protected:
   //--- process the preliminary queue
   bool              ProcessOrdersIncome(CExchangeContext &context);
   //--- process the queue of orders
   virtual bool      ProcessOrders(ExchangePendingOrdersArray &pending_orders,CTradeAccounts &accounts,CExchangeContext &context);
   //--- process an order from the queue of orders for processing
   virtual bool      ProcessOrder(ExchangePendingOrder &pending_order,CTradeAccounts &accounts,CExchangeContext &context)=0;
   //--- process quotes
   virtual bool      ProcessQuotes(CExchangeContext &context) { return(true); };
   //--- add volume to position
   virtual bool      PositionAddVolume(UINT64 login,INT64 volume,double price,double &profit);
   //--- get order direction
   bool              OrderIsBuy(UINT type_order);
   //--- get current time
   bool              TimeGet(INT64 &time_pf);
   //--- convert time from milliseconds into ticks
   bool              TimeConvert(const INT64 &time_msc,INT64 &time_pf);
  };
//+------------------------------------------------------------------+
