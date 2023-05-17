//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Feeder |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Subscription message header                                      |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct UniNewsSubscribeHeader
  {
   INT64             time_from;        // time, from which news are requested
   UINT              languages[32];    // subscription language identifiers
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| News subscription message                                        |
//+------------------------------------------------------------------+
class CUniNewsMsgSubscribe : public CUniNewsMsg
  {

private:
   //--- subscription data
   UniNewsSubscribeHeader *m_header;
   wchar_t          *m_keywords;       // subscription keywords

public:
   //--- constructor/destructor 
                     CUniNewsMsgSubscribe();
   virtual          ~CUniNewsMsgSubscribe();
   //--- initialization
   virtual bool      Initialize();
   //--- get values
   INT64             GetTimeFrom();
   UINT              GetLanguagesTotal();
   const UINT*       GetLanguages();
   LPCWSTR           GetKeywords();
   //--- set values
   bool              SetTimeFrom(const INT64 time_from);
   bool              SetLanguages(const UINT* languages,const UINT amount);
   bool              SetKeywords(LPCWSTR keywords);

protected:
   //--- parse buffer
   virtual void      ParseClear();
   virtual bool      ParseTag(TagHeader *tag);

private:
   //--- check subscription header
   bool              CheckHeader();
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
inline CUniNewsMsgSubscribe::CUniNewsMsgSubscribe()
   : CUniNewsMsg(),
     m_header(NULL),m_keywords(NULL)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
inline CUniNewsMsgSubscribe::~CUniNewsMsgSubscribe()
  {
  }
//+------------------------------------------------------------------+
//| Get subscription time                                            |
//+------------------------------------------------------------------+
inline INT64 CUniNewsMsgSubscribe::GetTimeFrom()
  {
//--- parse buffer, check message
   if(!ParseBuffer() || !m_header)
      return(0);
//--- return time
   return(m_header->time_from);
  }
//+------------------------------------------------------------------+
//|Get subscription languages amount                                 |
//+------------------------------------------------------------------+
inline UINT CUniNewsMsgSubscribe::GetLanguagesTotal()
  {
//--- parse buffer, check message
   if(!ParseBuffer() || !m_header)
      return(0);
//--- return languages amount
   return(_countof(m_header->languages));
  }
//+------------------------------------------------------------------+
//| Get subscription languages                                       |
//+------------------------------------------------------------------+
inline const UINT* CUniNewsMsgSubscribe::GetLanguages()
  {
//--- parse buffer, check message
   if(!ParseBuffer() || !m_header)
      return(NULL);
//--- return languages
   return(m_header->languages);
  }
//+------------------------------------------------------------------+
//| Get subscription keywords                                        |
//+------------------------------------------------------------------+
inline LPCWSTR CUniNewsMsgSubscribe::GetKeywords()
  {
//--- parse buffer
   if(!ParseBuffer())
      return(NULL);
//--- return keywords
   return(m_keywords);
  }
//+------------------------------------------------------------------+
//| Set subscription time                                            |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgSubscribe::SetTimeFrom(const INT64 time_from)
  {
//--- check header
   if(!CheckHeader())
      return(false);
//--- set subscription time
   m_header->time_from=time_from;
//--- successed 
   return(true);
  }
//+------------------------------------------------------------------+
//| Set subscription languages                                       |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgSubscribe::SetLanguages(const UINT* languages,const UINT amount)
  {
//--- check
   if(!languages || !amount || !CheckHeader())
      return(false);
//--- languages amount
   UINT langs_amount=amount;
   if(langs_amount>_countof(m_header->languages))
      langs_amount=_countof(m_header->languages);
//--- copy languages
   memcpy(m_header->languages,languages,langs_amount*sizeof(UINT));
//--- successed 
   return(true);
  }
//+------------------------------------------------------------------+
//| Set keywords                                                     |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgSubscribe::SetKeywords(LPCWSTR keywords)
  {
//--- check
   if(!keywords)
      return(false);
//--- add subscription keywords tag
   return(TagAdd(TAG_SUBSCRIBE_KEYWORDS,(const char*)keywords,(UINT)(wcslen(keywords)+1)*sizeof(wchar_t)));
  }
//+------------------------------------------------------------------+
//| Initialization                                                    |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgSubscribe::Initialize()
  {
//--- already initialized
   if(m_buffer && m_headtag && m_headtag->size)
      return(true);
//--- basic call
   if(!CUniNewsMsg::Initialize())
      return(false);
//--- set message type
   if(m_headtag)
      m_headtag->type=TAG_MSG_SUBSCRIBE;
//--- subscription header
   UniNewsSubscribeHeader header={0};
//--- add news header tag
   if(!TagAdd(TAG_SUBSCRIBE_HEADER,(char*)&header,sizeof(header)))
      return(false);
//--- if there are no subscription header in message, failed
   if(!m_header)
      return(false);
//--- fill header with default values
   m_header->time_from=0;
   ZeroMemory(m_header->languages,sizeof(m_header->languages));
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Clear message parsing data                            |
//+------------------------------------------------------------------+
inline void CUniNewsMsgSubscribe::ParseClear()
  {
//---basic call
   CUniNewsMsg::ParseClear();
//---reset pointers
   m_header=NULL;
   m_keywords=NULL;
  }
//+------------------------------------------------------------------+
//| Parse tag                                                        |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgSubscribe::ParseTag(TagHeader *tag)
  {
//--- check tag
   if(!tag || tag->size<sizeof(TagHeader))
      return(false);
//--- tag data is located behind of it's header
   char *data     =(char*)tag+sizeof(TagHeader);
   UINT  data_size=tag->size-sizeof(TagHeader);
//---check tag type
   switch(tag->type)
     {
      //--- subscription header
      case TAG_SUBSCRIBE_HEADER:
         if(data_size==sizeof(UniNewsSubscribeHeader))
            m_header=(UniNewsSubscribeHeader*)data;
         break;
         //--- keywords
      case TAG_SUBSCRIBE_KEYWORDS:
         if(data_size>=sizeof(wchar_t))
           {
            m_keywords=(wchar_t*)data;
            m_keywords[data_size/sizeof(wchar_t)-1]=L'\0';
           }
     }
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Check header                                                     |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgSubscribe::CheckHeader()
  {
//--- parse message, check header
   if(ParseBuffer() && m_header)
      return(true);
//--- initialize
   return(Initialize());
  }
//+------------------------------------------------------------------+
