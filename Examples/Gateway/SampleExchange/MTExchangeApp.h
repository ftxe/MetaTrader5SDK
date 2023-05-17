//+------------------------------------------------------------------+
//|                                     MetaTrader 5 Sample Exchange |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Class of application                                             |
//+------------------------------------------------------------------+
class CMTExchangeApp
  {

private:
   //--- trade dispatcher
   CTradeDispatcher  m_trade_dispatcher;
   //--- gateway connection context
   CExchangeContext  m_context;
   //--- gateway connection address
   wchar_t           m_address[MAX_PATH];

public:
   //--- constructor/destructor
                     CMTExchangeApp();
                    ~CMTExchangeApp();
   //--- initialize, run, shutdown, restart
   bool              Initialize(int argc,wchar_t** argv);
   void              Run();
   void              Shutdown();
   bool              Restart();

private:
   //--- process command line arguments
   bool              ProcessCommandLine(int argc,wchar_t** argv);
  };
//+------------------------------------------------------------------+
