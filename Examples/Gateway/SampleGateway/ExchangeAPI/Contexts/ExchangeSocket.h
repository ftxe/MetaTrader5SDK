//+------------------------------------------------------------------+
//|                                      MetaTrader 5 Sample Gateway |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Class of socket to connect to external trading system            |
//+------------------------------------------------------------------+
class CExchangeSocket
  {
private:
   //--- constants
   enum EnConstantsSocket
     {
      DEFAULT_SOCKET_TIMEOUT=10000           // socket default timeout (msec)
     };

private:
   //--- data
   volatile SOCKET   m_socket;               // used socket
   UINT              m_socket_timeout;       // socket timeout
   WSAEVENT          m_socket_event;         // socket event
   wchar_t           m_error_desc[MAX_PATH]; // description of socket error, if it will occur

public:
   //--- constructor/destructor
                     CExchangeSocket();
                    ~CExchangeSocket();
   //--- connect/disconnect
   bool              Connect(LPCWSTR server,const UINT port);
   void              Close();
   //--- connection state
   bool              Connected() const { return(m_socket!=INVALID_SOCKET); }
   //--- receive data
   bool              Read(char *buf,const UINT buf_max,UINT &buf_readed);
   //--- send data
   bool              Send(const char *buf,const UINT buf_size);
   //--- manage socket operation mode (blocking/non blocking)
   void              SetBlocking(const bool blocking);
   //--- set error description
   void              ErrorSet(const DWORD error_code);
   //--- set error description
   void              ErrorSet(LPCWSTR fmt,...);
   //--- get error description
   LPCWSTR           ErrorGet();
  };
//+------------------------------------------------------------------+
