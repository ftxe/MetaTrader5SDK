//+------------------------------------------------------------------+
//|                                     MetaTrader 5 Sample Exchange |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Trade account structure                                          |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct TradeAccount
  {
   UINT64            login;         // client's login
   double            balance;       // cleint's balance
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Array of accounts                                                |
//+------------------------------------------------------------------+
typedef TMTArray<TradeAccount> TradeAccountsArray;
//+------------------------------------------------------------------+
//| Class of trade accounts base                                     |
//+------------------------------------------------------------------+
class CTradeAccounts
  {

private:
   //--- constant parameters
   enum EnConstants
     {
      STARTING_BALANCE=100000000    // default balance
     };

private:
   //--- array of trade accoutns
   TradeAccountsArray m_trade_accounts;
   //--- object of synchronization of adding to trade instrument array
   CMTSync           m_sync;

public:
   //--- constructor/destructor
                     CTradeAccounts();
                    ~CTradeAccounts();
   //--- change balance for account
   void              BalanceAdd(UINT64 login, double volume);
   //--- geat balance of account
   double            BalanceGet(UINT64 login);
  };
//+------------------------------------------------------------------+
