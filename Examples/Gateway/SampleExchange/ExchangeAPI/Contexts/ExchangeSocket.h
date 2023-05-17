//+------------------------------------------------------------------+
//|                                     MetaTrader 5 Sample Exchange |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Class of server socket                                           |
//+------------------------------------------------------------------+
class CExchangeSocket
  {
private:
   //--- constants
   enum EnConstantsSocket
     {
      DEFAULT_SOCKET_TIMEOUT=10000           // socket default timeout (msec)
     };

   //--- description of socket error, if it will occur
   wchar_t           m_error_desc[MAX_PATH];
   //--- server socket
   SOCKET            m_socket_server;
   //--- client socket
   SOCKET            m_socket_client;
   UINT              m_socket_client_timeout;
   WSAEVENT          m_socket_client_event;

public:
   //--- constructor/destructor
                     CExchangeSocket();
                    ~CExchangeSocket();
   //--- manage incoming client connection
   bool              ClientConnect(LPCWSTR address,const UINT port);
   void              ClientClose()           { Close(m_socket_client);                  };
   bool              ClientConnected() const { return(m_socket_client!=INVALID_SOCKET); }
   //--- read/write socket data
   bool              ClientRead(char *buf,const UINT buf_max,UINT &buf_readed);
   bool              ClientSend(const char *buf,const UINT buf_size);
   //--- set error description
   void              ErrorSet(DWORD error_code);
   //--- set error description
   void              ErrorSet(LPCWSTR fmt,...);
   //--- get error description
   LPCWSTR           ErrorGet();

private:
   //--- manage server socket
   bool              ServerPrepare(LPCWSTR address,const UINT port);
   void              ServerClose()           { Close(m_socket_server);                  }
   //--- manage socket operation mode (blocking/non blocking)
   void              ClientSetBlocking(const bool blocking);
   //--- receive incoming client connection
   bool              ClientAccept();
   //--- close socket
   void              Close(SOCKET &sock);
  };
//+------------------------------------------------------------------+
