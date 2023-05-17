//+------------------------------------------------------------------+
//|                                      MetaTrader 5 Sample Gateway |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//---
#include "Common\ExchangeProtocol.h"
#include "Bases\ExchangeSymbols.h"
#include "Contexts\ExchangeContext.h"
//+------------------------------------------------------------------+
//| Class of external trading system API                             |
//+------------------------------------------------------------------+
class CExchangeAPI
  {
private:
   //--- constants
   enum EnConstants
     {
      TRANS_WAIT_TIME            =50      // transactions waiting time
     };

   //--- Gateway API interface
   CGateway         *m_gateway;
   //--- symbols base
   CExchangeSymbols  m_symbols;
   //--- context of connection to external trading system
   CExchangeContext  m_exchange_context;
   //--- thread of data processing
   CMTThread         m_thread;
   //--- thread's working flag
   volatile long     m_thread_workflag;

public:
   //--- constructor/destructor
                     CExchangeAPI(CGateway *gateway);
                    ~CExchangeAPI();
   //--- initialization/shutdown
   bool              Initialize(LPCWSTR server,LPCWSTR login,LPCWSTR password);
   void              Shutdown();
   //--- update state of connection context
   void              OnContextStateUpdate(const UINT state);
   //--- account data received
   bool              OnAccountData(const ExchangeAccountData &account_data);
   //--- check state of Gateway API
   bool              Check();
   //--- send order
   bool              SendOrder(const ExchangeOrder &order);
   //--- request for all account data
   bool              SendAccountDataRequest(UINT64 login);
   //--- find symbol by name
   bool              SymbolGet(LPCWSTR symbol_name,ExchangeSymbol &symbol);

private:
   //--- thread of data processing
   static UINT __stdcall ProcessThreadWrapper(LPVOID param);
   void              ProcessThread();
  };
//+------------------------------------------------------------------+
