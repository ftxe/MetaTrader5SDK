//+------------------------------------------------------------------+
//|                                     MetaTrader 5 Sample Exchange |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//--- trade instrument
#include "TradeAccounts.h"
#include "TradeInstrument.h"
//+------------------------------------------------------------------+
//| Class of trade dispatcher                                        |
//+------------------------------------------------------------------+
class CTradeDispatcher
  {

private:
   //--- event configuring constants
   enum EnConstants
     {
      DATA_WAIT_TIME=50     // new data wait time, ms
     };

private:
   //--- type: trade instrument array
   typedef TMTArray<CTradeInstrument*> TradeInstrumentsPtrArray;

private:
   //--- trade instrument array
   TradeInstrumentsPtrArray m_trade_instruments;
   //--- object of synchronization of adding to trade instrument array
   CMTSync           m_trade_instruments_sync;
   //--- trade accounts base
   CTradeAccounts    m_trade_accounts;
   //--- event of receiving data
   HANDLE            m_event;

public:
   //--- constructor/destructor
                     CTradeDispatcher();
                    ~CTradeDispatcher();
   //--- initialize/shutdown
   bool              Initialize();
   void              Shutdown();
   //--- data processing
   void              DataWait();
   bool              DataProcess(CExchangeContext &context);
   //--- add order to processing queue
   bool              OrderAdd(const ExchangeOrder &order);
   //--- export descriptions of trade instruments available for trading 
   bool              SymbolsSend(CExchangeContext &context);
   //--- send account data for login
   bool              AccountDataSend(CExchangeContext &context,UINT64 login);

private:
   //--- manage trade instruments
   bool              TradeInstrumentAdd(LPCWSTR symbol_name,
                                        LPCWSTR curr_base,
                                        LPCWSTR curr_profit,
                                        UINT exec_mode,
                                        UINT digits,
                                        double price_bid_init);
   CTradeInstrument* TradeInstrumentCreate(UINT execution_mode);
   void              TradeInstrumentFree(CTradeInstrument *trade_instrument);
   //--- manage the event
   bool              EventCreate();
   void              EventClose();
   bool              EventSet();
   bool              EventWait(const DWORD timeout);
   //--- search/sort trade instruments by name
   static int        SortTradeInstrumentsBySymbol(const void *left,const void *right);
   static int        SearchTradeInstrumentsBySymbol(const void *left,const void *right);
  };
//+------------------------------------------------------------------+
