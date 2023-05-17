//+------------------------------------------------------------------+
//|                                           MetaTrader 5 UniFeeder |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
class CDataSource;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
typedef TMTArray<MTTick> MTTickArray;
//+------------------------------------------------------------------+
//| Class of socket of connection to UniDDE Connector                |
//+------------------------------------------------------------------+
class CUniFeederSocket
  {
   //--- constants
   enum constants
     {
      THREAD_TIMEOUT   =60,           // timeout of thread completion, s
      READ_BUFFER_MAX  =1024*1024,    // maximal buffer size
      READ_BUFFER_STEP =16*1024,      // step of the buffer reallocation
      BUFFER_SIZE      =128*1024,     // size of the parsing buffer
      LOGIN_COUNT_MAX  =3             // maximal number of authorization attempts
     };

private:
   //--- reference to parent
   CDataSource      &m_parent;
   //--- access to Universal DDE Connector
   SOCKET            m_socket;         // socket
   //--- connection timeouts
   INT64             m_pingtime;       // server ping time
   INT64             m_closetime;      // time of closing by tomeout
   //--- buffer for the read string
   char              m_string[BUFFER_SIZE];
   //--- data for working with the socket
   char             *m_data;           // buffer of the socket data
   UINT              m_data_max;       // maximal length
   UINT              m_data_readed;    // amount of read data
   UINT              m_data_total;     // toral amount of data

public:
   //--- constructor/destructor
                     CUniFeederSocket(CDataSource &parent);
                    ~CUniFeederSocket();
   //--- connection/disconnection of Universal DDE Connector
   bool              Connect(LPCSTR server,LPCSTR login,LPCSTR password,LPCSTR symbols);
   void              Close(void);
   //--- receipt of a ticks
   bool              ReadTicks(MTTickArray &ticks);
   //--- check of the connection
   bool              Check();

private:
   //--- connection methods
   bool              ConnectDDE(LPCSTR server);
   bool              Login(LPCSTR login,LPCSTR password,LPCSTR symbols);
   //--- getting a tick from a string
   bool              ReadTick(LPSTR str,MTTick &tick);
   //--- processing of a data buffer
   bool              DataUpdate(void);
   bool              DataGetString(char *buf,const int maxlen);
   bool              DataCheck(void);
   //--- function of reading/writing to socket
   int               IsReadable(void);
   bool              ReadStringCheck(char *buf,const int maxlen,LPCSTR str,bool &found);
   bool              ReadString(char *buf,const int maxlen);
   bool              SendPing(void);
   bool              SendString(LPCSTR buf);
   //--- refreshing the socket closing time
   void              UpdateTime(void) { m_closetime=_time64(NULL)+THREAD_TIMEOUT; }
  };
//+------------------------------------------------------------------+
