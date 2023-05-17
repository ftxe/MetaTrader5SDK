//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Feeder |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| News message header structure                                    |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct UniNewsNewsHeader
  {
   INT64             datetime;         // publication date and time
   UINT              language;         // news language
   bool              priority;         // is news important?
   UINT              languages[32];    // list of languages the news is available for
  };
#pragma pack(pop)
//+------------------------------------------------------------------+
//| News message                                                     |
//+------------------------------------------------------------------+
class CUniNewsMsgNews : public CUniNewsMsg
  {

private:
   //--- news data
   UniNewsNewsHeader *m_header;        // news message header
   wchar_t          *m_category;       // news category
   wchar_t          *m_subject;        // news subject
   wchar_t          *m_body;           // mht formatted news body

public:
   //--- constructor/destructor
                     CUniNewsMsgNews();
   virtual          ~CUniNewsMsgNews();
   //--- initialization
   virtual bool      Initialize();
   //--- get values
   INT64             GetDatetime();
   UINT              GetLanguage();
   bool              GetPriority();
   UINT              GetLanguagesTotal();
   const UINT*       GetLanguages();
   LPCWSTR           GetCategory();
   LPCWSTR           GetSubject();
   LPCWSTR           GetBody();
   //--- set values
   bool              SetDatetime(const INT64 date);
   bool              SetLanguage(const UINT language);
   bool              SetPriority(const bool priority);
   bool              SetLanguages(const UINT* languages,const UINT amount);
   bool              SetCategory(LPCWSTR category);
   bool              SetSubject(LPCWSTR subject);
   bool              SetBody(LPCWSTR body);

protected:
   //--- parse buffer
   virtual void      ParseClear();
   virtual bool      ParseTag(TagHeader *tag);

private:
   //--- check news message header
   bool              CheckHeader();
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
inline CUniNewsMsgNews::CUniNewsMsgNews()
 : CUniNewsMsg(),
   m_header(NULL),m_category(NULL),m_subject(NULL),m_body(NULL)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
inline CUniNewsMsgNews::~CUniNewsMsgNews()
  {
  }
//+------------------------------------------------------------------+
//| Get publication date and time                                    |
//+------------------------------------------------------------------+
inline INT64 CUniNewsMsgNews::GetDatetime()
  {
//--- parse buffer, check header
   if(!ParseBuffer() || !m_header)
      return(0);
//--- return datetime
   return(m_header->datetime);
  }
//+------------------------------------------------------------------+
//| Get news language                                                |
//+------------------------------------------------------------------+
inline UINT CUniNewsMsgNews::GetLanguage()
  {
//--- parse buffer, check header
   if(!ParseBuffer() || !m_header)
      return(0);
//--- return news language
   return(m_header->language);
  }
//+------------------------------------------------------------------+
//| Get news priority flag                                           |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::GetPriority()
  {
//--- parse buffer, check message
   if(!ParseBuffer() || !m_header)
      return(false);
//--- return ne
   return(m_header->priority);
  }
//+------------------------------------------------------------------+
//| Get number of languages the news is available for                |
//+------------------------------------------------------------------+
inline UINT CUniNewsMsgNews::GetLanguagesTotal()
  {
//--- parse buffer, check message
   if(!ParseBuffer() || !m_header)
      return(0);
//--- return news amount
   return(_countof(m_header->languages));
  }
//+------------------------------------------------------------------+
//| Get list of languages the news is available for                  |
//+------------------------------------------------------------------+
inline const UINT* CUniNewsMsgNews::GetLanguages()
  {
//--- parse buffer, check message
   if(!ParseBuffer() || !m_header)
      return(NULL);
//--- return languages list
   return(m_header->languages);
  }
//+------------------------------------------------------------------+
//| Get news category                                                |
//+------------------------------------------------------------------+
inline LPCWSTR CUniNewsMsgNews::GetCategory()
  {
//--- parse buffer
   if(!ParseBuffer())
      return(NULL);
//--- return category
   return(m_category);
  }
//+------------------------------------------------------------------+
//| Get news subject                                                 |
//+------------------------------------------------------------------+
inline LPCWSTR CUniNewsMsgNews::GetSubject()
  {
//--- parse buffer
   if(!ParseBuffer())
      return(NULL);
//--- return news subject
   return(m_subject);
  }
//+------------------------------------------------------------------+
//| Get news body                                                    |
//+------------------------------------------------------------------+
inline LPCWSTR CUniNewsMsgNews::GetBody()
  {
//--- parse buffer
   if(!ParseBuffer())
      return(NULL);
//--- return news body
   return(m_body);
  }
//+------------------------------------------------------------------+
//| Set news publication time                                        |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::SetDatetime(const INT64 datetime)
  {
//--- check header
   if(!CheckHeader())
      return(false);
//--- set time
   m_header->datetime=datetime;
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Set news language                                                |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::SetLanguage(const UINT language)
  {
//--- check header
   if(!CheckHeader())
      return(false);
//--- set news language
   m_header->language=language;
//--- successed 
   return(true);
  }
//+------------------------------------------------------------------+
//| Set news priority flag                                           |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::SetPriority(const bool priority)
  {
//--- check header
   if(!CheckHeader())
      return(false);
//--- set news priority flag
   m_header->priority=priority;
//--- successed 
   return(true);
  }
//+------------------------------------------------------------------+
//| Set list of languages the news is available for                  |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::SetLanguages(const UINT* languages,const UINT amount)
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
//| Set news category                                                |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::SetCategory(LPCWSTR category)
  {
//--- check
   if(!category)
      return(false);
//--- add news category tag
   return(TagAdd(TAG_NEWS_CATEGORY,(const char*)category,(UINT)(wcslen(category)+1)*sizeof(wchar_t)));
  }
//+------------------------------------------------------------------+
//| Set enws subject                                                 |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::SetSubject(LPCWSTR subject)
  {
//--- check
   if(!subject)
      return(false);
//--- add news subject tag
   return(TagAdd(TAG_NEWS_SUBJECT,(const char*)subject,(UINT)(wcslen(subject)+1)*sizeof(wchar_t)));
  }
//+------------------------------------------------------------------+
//| Set news body                                                    |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::SetBody(LPCWSTR body)
  {
//--- check
   if(!body)
      return(false);
//--- add news body tag
   return(TagAdd(TAG_NEWS_BODY,(const char*)body,(UINT)(wcslen(body)+1)*sizeof(wchar_t)));
  }
//+------------------------------------------------------------------+
//| Initialization                                                    |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::Initialize()
  {
//--- already initialized
   if(m_buffer && m_headtag && m_headtag->size)
      return(true);
//---basic call
   if(!CUniNewsMsg::Initialize())
      return(false);
//--- set message type
   if(m_headtag)
      m_headtag->type=TAG_MSG_NEWS;
//--- news message header
   UniNewsNewsHeader header={0};
//--- add news header tag
   if(!TagAdd(TAG_NEWS_HEADER,(char*)&header,sizeof(header)))
      return(false);
//--- if there are no header in message, failed
   if(!m_header)
      return(false);
//---if there are no header in message, failed
   m_header->datetime=0;
   m_header->language=LANG_NEUTRAL;
   m_header->priority=false;
   ZeroMemory(m_header->languages,sizeof(m_header->languages));
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Clear message parsing data                            |
//+------------------------------------------------------------------+
inline void CUniNewsMsgNews::ParseClear()
  {
//---basic call
   CUniNewsMsg::ParseClear();
//---reset pointers
   m_header=NULL;
   m_category=NULL;
   m_subject=NULL;
   m_body=NULL;
  }
//+------------------------------------------------------------------+
//| Parse tag                                                        |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::ParseTag(TagHeader *tag)
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
      //--- news header
      case TAG_NEWS_HEADER:
         if(data_size==sizeof(UniNewsNewsHeader))
            m_header=(UniNewsNewsHeader*)data;
         break;
         //--- news category
      case TAG_NEWS_CATEGORY:
         if(data_size>=sizeof(wchar_t))
           {
            m_category=(wchar_t*)data;
            m_category[data_size/sizeof(wchar_t)-1]=L'\0';
           }
         break;
         //--- news subject
      case TAG_NEWS_SUBJECT:
         if(data_size>=sizeof(wchar_t))
           {
            m_subject=(wchar_t*)data;
            m_subject[data_size/sizeof(wchar_t)-1]=L'\0';
           }
         break;
         //--- news body
      case TAG_NEWS_BODY:
         if(data_size>=sizeof(wchar_t))
           {
            m_body=(wchar_t*)data;
            m_body[data_size/sizeof(wchar_t)-1]=L'\0';
           }
         break;
     }
//--- successed
   return(true);
  }
//+------------------------------------------------------------------+
//| Check header                                                     |
//+------------------------------------------------------------------+
inline bool CUniNewsMsgNews::CheckHeader()
  {
//--- parse message, check header
   if(ParseBuffer() && m_header)
      return(true);
//--- initialize
   return(Initialize());
  }
//+------------------------------------------------------------------+
