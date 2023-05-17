//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Server |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Logger class                                                     |
//+------------------------------------------------------------------+
class CLogger
  {

private:
   //--- internal constants of class
   enum EnLoggerConstants
     {
      PREBUF_SIZE=16*1024                       // preliminary buffer size
     };

private:
   CMTSync           m_sync;                    // synchronization
   wchar_t           m_path[MAX_PATH];          // path to logs folder
   wchar_t           m_prebuf[PREBUF_SIZE];     // message buffer

public:
                     CLogger();
                    ~CLogger();
   //--- initialization
   bool              Initialize();
   //--- print logs
   void              Out(int code,LPCWSTR msg,...);
   void              OutString(int code,LPCWSTR msg);

private:
   //--- print logs
   void              OutArgPtr(int code,LPCWSTR msg,va_list arg_ptr);
  };
extern CLogger ExtLogger;
//+------------------------------------------------------------------+
