//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Server |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Class of application                                             |
//+------------------------------------------------------------------+
class CMTUniNewsServerApp
  {

private:
   //--- news source
   CUniNewsSource    m_source;
   //--- news server port
   CUniNewsServerPort m_server_port;   
   //--- news server address
   wchar_t           m_address[MAX_PATH];
   //--- client connections login and password
   wchar_t           m_login[64];
   wchar_t           m_password[64];
   //--- news path 
   wchar_t           m_news_path[64];

public:
   //--- constructor/destructor
                     CMTUniNewsServerApp();
                    ~CMTUniNewsServerApp();
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
