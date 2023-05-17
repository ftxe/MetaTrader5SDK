//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Feeder |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Ping message                                                     |
//+------------------------------------------------------------------+
class CUniNewsMsgPing : public CUniNewsMsg
  {

public:
   //--- constructor/destructor
                     CUniNewsMsgPing();
   virtual          ~CUniNewsMsgPing();
   //--- initialization
   virtual bool      Initialize();
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
inline CUniNewsMsgPing::CUniNewsMsgPing() : CUniNewsMsg()
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
inline CUniNewsMsgPing::~CUniNewsMsgPing()
  {
  }
//+------------------------------------------------------------------+
//| Initialization                                                    |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgPing::Initialize()
  {
//--- already initialized
   if(m_buffer && m_headtag && m_headtag->size)
      return(true);
//---basic call
   if(!CUniNewsMsg::Initialize())
      return(false);
//--- set message type
   if(m_headtag)
      m_headtag->type=TAG_MSG_PING;
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
