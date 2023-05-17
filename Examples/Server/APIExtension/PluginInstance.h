//+------------------------------------------------------------------+
//|                                       MetaTrader 5 API Extension |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Plugin implementation                                            |
//+------------------------------------------------------------------+
class CPluginInstance : public IMTServerPlugin,
                        public IMTCustomSink
  {
public:
   //--- Command identificator
   enum EnCommand
     {
      COMMAND_RATE_GET_BUY   =1,
      COMMAND_RATE_GET_SELL  =2,
      COMMAND_POSITIONS_TOTAL=3,
      COMMAND_ORDERS_TOTAL   =4,
      COMMAND_USERS_TOTAL    =5,
      COMMAND_RATE_GET_BUY_JSON     =6,
      COMMAND_RATE_GET_SELL_JSON    =7,
      COMMAND_POSITIONS_TOTAL_JSON  =8,
      COMMAND_ORDERS_TOTAL_JSON     =9,
      COMMAND_USERS_TOTAL_JSON      =10,
     };
   //--- const
   enum EnConstants
     {
      RATE_DIGITS    =5,   // digits for rate
     };
   //--- logins array
   typedef TMTArray<UINT64,1024> LoginArray;

private:
   IMTServerAPI     *m_api;

public:
                     CPluginInstance(void);
                    ~CPluginInstance(void);
   //--- IMTServerPlugin methods
   virtual void      Release(void);
   virtual MTAPIRES  Start(IMTServerAPI *server);
   virtual MTAPIRES  Stop(void);
   //--- from IMTCustomSink
   virtual MTAPIRES  HookManagerCommand(const UINT64         session,
                                        LPCWSTR              ip,
                                        const IMTConManager *manager,
                                        IMTByteStream       *indata,
                                        IMTByteStream       *outdata);
   //--- HookWebAPICommand
   MTAPIRES          HookWebAPICommand(const UINT64         session,
                                       LPCWSTR              ip,
                                       const IMTConManager *manager,
                                       LPCWSTR              command,
                                       IMTByteStream       *indata,
                                       IMTByteStream       *outdata);
private:
   UINT              CommandGetId(LPCWSTR command);
   //--- user commands
   MTAPIRES          PositionGetTotal(const MTAPISTR group,UINT &total);
   MTAPIRES          OrdersGetTotal(const MTAPISTR group,UINT &total);
   MTAPIRES          UsersGetTotal(const MTAPISTR group_mask,UINT &total);
   MTAPIRES          UserLogins(const MTAPISTR group_mask,LoginArray &logins);
  };
//+------------------------------------------------------------------+
