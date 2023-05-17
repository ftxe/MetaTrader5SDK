//+------------------------------------------------------------------+
//|                                      MetaTrader 5 Sample Gateway |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//---
#include "Bases\Gateway.h"
//+------------------------------------------------------------------+
//| Class of application                                             |
//+------------------------------------------------------------------+
class CMTGatewayApp : public IMTGatewaySink
  {

private:
   //--- constants
   enum
     {
      TIMEOUT_CHECK_STATE=500,                       // period of check
     };

private:
   //--- Gateway API library
   CMTGatewayAPIFactory m_api_factory;
   //--- Gateway API interface
   IMTGatewayAPI    *m_api_gateway;
   //--- gateway
   CGateway         *m_gateway;
   //--- working flag
   volatile long     m_working_flag;

public:
   //--- constructor/destructor
                     CMTGatewayApp();
                    ~CMTGatewayApp();
   //--- set gateway description
   static void       Info(MTGatewayInfo &info);
   //--- initialize, run, shutdown
   bool              Initialize(int argc,wchar_t** argv);
   bool              Run();
   void              Shutdown();
   //--- get/set working flag
   long              Working()              { return(InterlockedExchangeAdd(&m_working_flag,0));         }
   long              Working(long working_flag) { return(InterlockedExchange(&m_working_flag,working_flag)); }
   //--- notify of server disconnect
   virtual void      OnServerDisconnect(LPCWSTR address,const UINT type,const UINT64 login);
   //--- handlers of Gateway API notifications
   virtual void      OnGatewayConfig(const UINT64 login,const IMTConGateway* config);
   virtual void      OnGatewayStart();
   virtual void      OnGatewayStop();
   virtual void      OnGatewayShutdown(const UINT64 login);
   //--- notify of result of receiving request for processing
   virtual void      OnDealerLock(const MTAPIRES retcode,const IMTRequest *request,
                                  const IMTUser *user,const IMTAccount *account,
                                  const IMTOrder *order,const IMTPosition *position);
   //--- notify of result of processing request response
   virtual void      OnDealerAnswer(const MTAPIRES retcode,const IMTConfirm *confirm);
   //--- external trading system control hooks
   virtual MTAPIRES  HookGatewayPositionsRequest();
   virtual MTAPIRES  HookGatewayAccountRequest(UINT64 login,LPCWSTR account_id);
  };
//+------------------------------------------------------------------+
