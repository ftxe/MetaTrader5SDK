//+------------------------------------------------------------------+
//|                                      MetaTrader 5 Sample Gateway |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//---
#include "ExchangeSocket.h"
//+------------------------------------------------------------------+
//| Preliminary declarations                                         |
//+------------------------------------------------------------------+
class  CExchangeAPI;
//+------------------------------------------------------------------+
//| Class of connection to external trading system context           |
//+------------------------------------------------------------------+
class CExchangeContext : public CExchangeContextBase
  {

public:
   //--- context state
   enum EnExchangeContextState
     {
      STATE_STOPPED           =0x00,                     // stopped thread of processing connection messages
      STATE_STARTED           =0x01,                     // started thread of processing connection messages
      STATE_SYNCHRONIZED      =0x02,                     // connection fully synchronized
      STATE_FIRST             =STATE_STOPPED,
      STATE_LAST              =STATE_SYNCHRONIZED
     };

private:
   //--- context timeouts
   enum EnExchangeContextTimeouts
     {
      THREAD_TIMEOUT          =20,                       // thread timout, sec.
      LOGOUT_TIME             =2000,                     // logout time, msec
      LOGOUT_WAIT             =50                        // logout message wait time, msec
     };
   //--- constants
   enum EnConstants
     {
      THREAD_SLEEP            =10,                       // thread sleep time, msec
      MAX_BUFFER_SEND         =256*1024*1024,            // maximum size of buffer for sending
      MAX_BUFFER_RECEIVE      =10*1024,                  // maximum size of receipt buffer
      HEARBEAT_TIMEOUT        =10                        // hearbeat timeout
     };
   //--- array of ticks
   typedef TMTArray<ExchangeTick> ExchangeTicksArray;
   //--- array of books
   typedef TMTArray<ExchangeBook> ExchangeBooksArray;

private:
   //--- interface of external trading system
   CExchangeAPI     &m_exchange_api;
   //--- connection socket
   CExchangeSocket  *m_socket;
   //--- connection parameters
   wchar_t           m_address[MAX_PATH];
   UINT              m_port;
   //--- login and password
   wchar_t           m_login[64];
   wchar_t           m_password[64];
   //--- buffers of packets for sending
   CMTMemPack        m_buffer_send;
   CMTMemPack        m_buffer_send_tmp;
   //--- synchronizer of access to packets for sending
   CMTSync           m_buffer_send_sync;
   //--- buffer of receipt packets
   char             *m_buffer_receive;
   UINT              m_buffer_receive_max;
   //--- protocol
   CExchangeProtocol m_exchange_protocol;
   //--- thread of messages processing
   CMTThread         m_thread;
   //--- flag of processing thread
   volatile long     m_thread_workflag;
   //--- time of last activity of processing thread
   volatile INT64    m_thread_checktime;
   //--- state of connection context
   volatile long     m_state;
   //--- transactions of trade symbols
   ExchangeSymbolsArray m_trans_symbols;
   ExchangeSymbolsArray m_trans_symbols_tmp;
   //--- transactions of ticks
   ExchangeTicksArray m_trans_ticks;
   ExchangeTicksArray m_trans_ticks_tmp;
   //--- transactions of books
   ExchangeBooksArray m_trans_books;
   ExchangeBooksArray m_trans_books_tmp;
   //--- transactions of orders
   ExchangeOrdersArray m_trans_orders;
   ExchangeOrdersArray m_trans_orders_tmp;
   //--- transactions of deals
   ExchangeDealsArray m_trans_deals;
   ExchangeDealsArray m_trans_deals_tmp;
   //--- synchronizer of access to transactions
   CMTSync           m_trans_sync;
   //--- login state
   volatile long     m_logon;
   //--- heartbeat time
   volatile INT64    m_heartbeat_time;

public:
   //--- constructor/destructor
                     CExchangeContext(CExchangeAPI &exchange_api);
                    ~CExchangeContext();
   //--- initialization/shutdown
   bool              Initialize(LPCWSTR address,const UINT port,LPCWSTR login,LPCWSTR password);
   void              Shutdown();
   //--- state of connection context
   UINT              StateSet(UINT state);
   UINT              StateGet();
   //--- send data
   virtual bool      Send(const char *buffer,const UINT buffer_size);
   //--- update activity time
   INT64             UpdateTime(const INT64 timeout)     { return(m_thread_checktime=_time64(NULL)+timeout);      }
   //--- check context
   bool              Check(const INT64 curtime);
   //--- process received transactions
   bool              TransApply(CExchangeSymbols &symbols,bool &trans_applied);
   bool              TransApplySymbols(CExchangeSymbols &symbols_base,ExchangeSymbolsArray &trans_symbols,bool &trans_applied);
   bool              TransApplyTicks(CExchangeSymbols &symbols_base,ExchangeTicksArray &trans_ticks,bool &trans_applied);
   bool              TransApplyBooks(CExchangeSymbols &symbols_base,ExchangeBooksArray &trans_books,bool &trans_applied);
   bool              TransApplyOrders(CExchangeSymbols &symbols_base,ExchangeOrdersArray &trans_orders,bool &trans_applied);
   bool              TransApplyDeals(CExchangeSymbols &symbols_base,ExchangeDealsArray &trans_deals,bool &trans_applied);
   //--- process login message
   virtual bool      OnMessageLogin(const ExchangeMsgLogin &msg);
   //--- process logout message
   virtual bool      OnMessageLogout(const ExchangeMsgLogout &msg);
   //--- process message with symbol settings
   virtual bool      OnMessageSymbol(const ExchangeMsgSymbol &msg);
   //--- process message with tick
   virtual bool      OnMessageTick(const ExchangeMsgTick &msg);
   //--- process message with book
   virtual bool      OnMessageBook(const ExchangeMsgBook &msg);
   //--- process message with order
   virtual bool      OnMessageOrder(const ExchangeMsgOrder &msg);
   //--- process message with deal
   virtual bool      OnMessageDeal(const ExchangeMsgDeal &msg);
   //--- process message with account data request
   virtual bool      OnMessageAccountDataRequest(const ExchangeMsgAccountDataRequest &msg) { return(true); }
   //--- process message with account data
   virtual bool      OnMessageAccountData(const ExchangeMsgAccountData &msg);
   //--- process HeartBeat message
   virtual bool      OnMessageHeartBeat(const ExchangeMsgHeartBeat &msg);
   //--- send login
   bool              SendLogin();
   //--- send logout
   bool              SendLogoutSync();
   //--- send trade order
   bool              SendOrder(const ExchangeOrder &order);
   //--- send request for account data
   bool              SendAccountDataRequest(UINT64 login);
   //--- send HeartBeat
   bool              SendHeartBeat(const INT64 curtime);

private:
   //--- initialize buffers of data packets
   bool              InitializeBuffers();
   //--- notification methods
   bool              OnConnect();
   bool              OnReceive(char *buffer,const UINT buffer_size);
   //--- thread of messages processing
   static UINT __stdcall ProcessThreadWrapper(LPVOID param);
   void              ProcessThread();
   //--- send/receive data
   bool              SendCheck(bool &data_processed);
   bool              ReceiveCheck(bool &data_processed);
   //--- get/set logon-state
   bool              IsLogon()        { return(InterlockedExchangeAdd(&m_logon,0)>0);      }
   bool              LogonSet(bool logon) { return(InterlockedExchange(&m_logon,logon?1:0)>0); }
  };
//+------------------------------------------------------------------+
