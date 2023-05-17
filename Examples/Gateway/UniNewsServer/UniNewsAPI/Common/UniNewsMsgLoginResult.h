//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Feeder |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Authorization result message header structure                    |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct UniNewsLoginResultHeader
  {
   UINT              version;          // server protocol version
   UINT              result;           // authorization result
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| Authorization result message                                     |
//+------------------------------------------------------------------+
class CUniNewsMsgLoginResult : public CUniNewsMsg
  {

public:
   enum EnLoginResult
     {
      RESULT_SUCCESSFUL          =0,         // authorization successful
      RESULT_COMMON_ERROR        =1,         // common error
      RESULT_INVALID_PASSWORD    =2,         // invalid login or password
      RESULT_UNSUPPORTED_PROTOCOL=3          // unsupported client protocol version
     };

private:
   //--- authorization result header
   UniNewsLoginResultHeader *m_header;

public:
   //--- constructor/destructor
                     CUniNewsMsgLoginResult();
   virtual          ~CUniNewsMsgLoginResult();
   //--- initialization
   virtual bool      Initialize();
   //--- get values
   UINT              GetVersion();
   UINT              GetResult();
   //--- set values
   bool              SetVersion(const UINT version);
   bool              SetResult(const UINT result);

protected:
   //--- buffer size
   virtual void      ParseClear();
   virtual bool      ParseTag(TagHeader *tag);

private:
   //--- check message header
   bool              CheckHeader();
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
inline CUniNewsMsgLoginResult::CUniNewsMsgLoginResult()
 : CUniNewsMsg(),
   m_header(NULL)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
inline CUniNewsMsgLoginResult::~CUniNewsMsgLoginResult()
  {
  }
//+------------------------------------------------------------------+
//| Get server protocol version                                      |
//+------------------------------------------------------------------+
inline UINT CUniNewsMsgLoginResult::GetVersion()
  {
//--- parse buffer, check header
   if(!ParseBuffer() || !m_header)
      return(0);
//--- return protocol version
   return(m_header->version);
  }
//+------------------------------------------------------------------+
//| Get authorization result                                         |
//+------------------------------------------------------------------+
inline UINT CUniNewsMsgLoginResult::GetResult()
  {
//--- parse buffer, check header
   if(!ParseBuffer() || !m_header)
      return(RESULT_COMMON_ERROR);
//--- return result
   return(m_header->result);
  }
//+------------------------------------------------------------------+
//|Set server protocol version                                       |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgLoginResult::SetVersion(const UINT version)
  {
//--- check
   if(!CheckHeader())
      return(false);
//--- set protocol version
   m_header->version=version;
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Set authorization result                                         |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgLoginResult::SetResult(const UINT result)
  {
//--- check header
   if(!CheckHeader())
      return(false);
//--- set result
   m_header->result=result;
//--- successed 
   return(true);
  }
//+------------------------------------------------------------------+
//| Clear message parsing data                            |
//+------------------------------------------------------------------+
inline void CUniNewsMsgLoginResult::ParseClear()
  {
//--- basi call
   CUniNewsMsg::ParseClear();
//--- reset pointers
   m_header=NULL;
  }
//+------------------------------------------------------------------+
//| Initialization                                                   |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgLoginResult::Initialize()
  {
//--- already initialized
   if(m_buffer && m_headtag && m_headtag->size)
      return(true);
//--- basic call
   if(!CUniNewsMsg::Initialize())
      return(false);
//--- set message type
   if(m_headtag)
      m_headtag->type=TAG_MSG_LOGIN_RESULT;
//--- login result header
   UniNewsLoginResultHeader header={0};
//--- add header tag
   if(!TagAdd(TAG_LOGIN_RESULT_HEADER,(char*)&header,sizeof(header)))
      return(false);
//--- if there is no header in message, failed
   if(!m_header)
      return(false);
//--- fill header with default values
   m_header->version=UNINEWS_PROTOCOL_VERSION;
   m_header->result=RESULT_COMMON_ERROR;
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Parse tag                                                        |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgLoginResult::ParseTag(TagHeader *tag)
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
      //--- login result header      
      case TAG_LOGIN_RESULT_HEADER:
         if(data_size==sizeof(UniNewsLoginResultHeader))
            m_header=(UniNewsLoginResultHeader*)data;
         break;
     }
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Check message header                                             |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgLoginResult::CheckHeader()
  {
//--- parse message, check login result header
   if(ParseBuffer() && m_header)
      return(true);
//--- initialize message
   return(Initialize());
  }
//+------------------------------------------------------------------+
