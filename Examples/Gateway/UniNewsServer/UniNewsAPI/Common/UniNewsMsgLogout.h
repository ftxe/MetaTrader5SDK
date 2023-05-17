//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Feeder |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Logout message header struture                                   |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct UniNewsLogoutHeader
  {
   UINT              reason;           // logout reason
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Logout message                                                   |
//+------------------------------------------------------------------+
class CUniNewsMsgLogout : public CUniNewsMsg
  {

public:
   //--- logout reasons
   enum EnReason
     {
      LOGOUT_REASON_UNKNOWN             =0,  // reason is unknown
      LOGOUT_REASON_UNSUPPORTED_PROTOCOL=1,  // unsupported remote part protocol version
      LOGOUT_REASON_CLIENT              =2,  // client closed connection
      LOGOUT_REASON_SERVER              =3   // server closed connection
     };

private:
   //--- logout header
   UniNewsLogoutHeader *m_header;

public:
   //--- constructor/destructor
                     CUniNewsMsgLogout();
   virtual          ~CUniNewsMsgLogout();
   //--- initialization
   virtual bool      Initialize();
   //--- get values
   UINT              GetReason();
   //--- set values
   bool              SetReason(const UINT reason);

protected:
   //--- parse buffer
   virtual void      ParseClear();
   virtual bool      ParseTag(TagHeader *tag);

private:
   //--- check ,essage header
   bool              CheckHeader();
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
inline CUniNewsMsgLogout::CUniNewsMsgLogout()
 : CUniNewsMsg(),
   m_header(NULL)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
inline CUniNewsMsgLogout::~CUniNewsMsgLogout()
  {
  }
//+------------------------------------------------------------------+
//| Get logout reason                                                |
//+------------------------------------------------------------------+
inline UINT CUniNewsMsgLogout::GetReason()
  {
//--- parse buffer, check header
   if(!ParseBuffer() || !m_header)
      return(LOGOUT_REASON_UNKNOWN);
//--- return reason
   return(m_header->reason);
  }
//+------------------------------------------------------------------+
//| Set logout reason                                                |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgLogout::SetReason(const UINT reason)
  {
//--- check header
   if(!CheckHeader())
      return(false);
//--- set logout reason
   m_header->reason=reason;
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Initialization                                                    |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgLogout::Initialize()
  {
//--- already initialized
   if(m_buffer && m_headtag && m_headtag->size)
      return(true);
//--- basic call
   if(!CUniNewsMsg::Initialize())
      return(false);
//--- set message type
   if(m_headtag)
      m_headtag->type=TAG_MSG_LOGOUT;
//--- logout header
   UniNewsLogoutHeader header={0};
//--- add header tag
   if(!TagAdd(TAG_LOGOUT_HEADER,(char*)&header,sizeof(header)))
      return(false);
//--- if there are no logout header in message, failed
   if(!m_header)
      return(false);
//--- fill header with default values
   m_header->reason=LOGOUT_REASON_UNKNOWN;
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Clear message parsing data                            |
//+------------------------------------------------------------------+
inline void CUniNewsMsgLogout::ParseClear()
  {
//--- basic call
   CUniNewsMsg::ParseClear();
//--- reset pointers
   m_header=NULL;
  }
//+------------------------------------------------------------------+
//| Parse tag                                                        |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgLogout::ParseTag(TagHeader *tag)
  {
//--- check tag
   if(!tag || tag->size<sizeof(TagHeader))
      return(false);
//--- tag data is located behind of tag header
   char *data     =(char*)tag+sizeof(TagHeader);
   UINT  data_size=tag->size-sizeof(TagHeader);
//--- check tag type
   switch(tag->type)
     {
      //--- logout header      
      case TAG_LOGOUT_HEADER:
         if(data_size==sizeof(UniNewsLogoutHeader))
            m_header=(UniNewsLogoutHeader*)data;
         break;
     }
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Check header                                                     |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgLogout::CheckHeader()
  {
//--- parse message, check header
   if(ParseBuffer() && m_header)
      return(true);
//--- initialize message
   return(Initialize());
  }
//+------------------------------------------------------------------+
