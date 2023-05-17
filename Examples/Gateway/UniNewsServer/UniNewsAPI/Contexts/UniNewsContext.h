//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Server |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Client connection context                                        |
//+------------------------------------------------------------------+
class CUniNewsContext : public CUniNewsContextBase
  {
private:
   //--- constants
   enum EnConstants
     {
      THREAD_TIMEOUT       =20,     // thread timeout, s
      THREAD_SLEEP         =10,     // thread sleep time
      RECEIVE_BUFFER_SIZE  =64*1024 // receive buffer size, bytes
     };

private:
   //--- news source reference
   CUniNewsSource   &m_data_source;
   //--- socket
   volatile SOCKET   m_socket;
   volatile INT64    m_close_time;
   static volatile long m_connect_id;
   CMTStr64          m_ip;
   //--- thread
   CMTThread         m_thread;
   volatile bool     m_workflag;
   //--- pointer to next context in list
   CUniNewsContext  *m_next;
   //--- protocol
   CUniNewsProtocol  m_protocol;
   //--- last data send/receive time
   volatile INT64    m_receive_checktime;
   volatile INT64    m_send_checktime;
   //--- buffers
   CMTMemPack        m_buffer_send;
   CMTMemPack        m_buffer_receive;
   CMTMemPack        m_buffer_tmp;
   CMTSync           m_buffer_sync;
   //--- login and password
   wchar_t           m_login[64];
   wchar_t           m_password[64];
   //--- subscription parameters
   INT64             m_subscribe_from;
                     TMTArray<wchar_t[64]>m_subscribe_keywords;
   UINT              m_subscribe_languages[32];
   volatile bool     m_subscribed;

public:
                     CUniNewsContext(SOCKET sock,const UINT ip,CUniNewsSource &data_source);
   virtual          ~CUniNewsContext(void);
   //--- initialization/shutdown
   bool              Initialize(LPCWSTR login,LPCWSTR password);
   void              Shutdown();
   //--- update activity time
   void              UpdateTime(const INT64 timeout)     { InterlockedExchange64(&m_close_time,_time64(NULL)+timeout); }
   //--- check context
   bool              Check(const INT64 curtime);
   //--- list
   CUniNewsContext*  Next()                              { return(m_next); }
   void              Next(CUniNewsContext* next)         { m_next=next;    }
   //--- connection state
   bool              Connected(void) const               { return(m_socket!=INVALID_SOCKET); }
   //--- close socket
   void              CloseConnection(void);
   //--- send data
   virtual bool      MessageSend(CUniNewsMsg *msg);
   //--- process login message
   virtual bool      OnMessageLogin(CUniNewsMsgLogin *msg);
   //--- process logout message
   virtual bool      OnMessageLogout(CUniNewsMsgLogout *msg);
   //--- process subscription message
   virtual bool      OnMessageSubscribe(CUniNewsMsgSubscribe *msg);
   //--- send login result message
   bool              SendLoginResult(const UINT result=CUniNewsMsgLoginResult::RESULT_COMMON_ERROR);
   //--- send logout message
   bool              SendLogout(const UINT reason=CUniNewsMsgLogout::LOGOUT_REASON_UNKNOWN);
   //--- send news message
   bool              SendNews(CUniNewsMsgNews *news,LPCWSTR keywords,bool history_news=false);
   //--- send ping message
   bool              SendPing();

private:
   //--- check login
   bool              CheckLogin(LPCWSTR login,LPCWSTR password_hash);
   //--- thread of connection processing
   static UINT __stdcall ProcessThreadWrapper(LPVOID param);
   void              ProcessThread();
   //--- receive data
   bool              Receive(bool &data_processed);
   //--- send data
   bool              Send(bool &data_processed);
   //--- check if readible
   int               IsReadible();
   //--- read socket data
   bool              Read(char *buf,int buf_max,int &buf_readed,DWORD* err_code=NULL);
   //--- write socket data
   bool              Write(const char *buf,const UINT len,DWORD* err_code=NULL);
  };
//+------------------------------------------------------------------+
