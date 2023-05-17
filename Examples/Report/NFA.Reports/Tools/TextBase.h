//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Text based CSV database                                          |
//+------------------------------------------------------------------+
class CTextBase
  {
private:
   enum constants
     {
      BASE_GROW_STEP=512*KB,           // file grow step
      BASE_READ_SIZE=32*KB,            // read buffer size
      BADE_IDEN_SIZE=256,              // id buffer size
     };

private:
   //--- common
   UINT64            m_id;             // last record id
   char              m_delimiter;      // delimiter
   //--- file base
   CMTFile           m_file;           // file
   UINT64            m_file_size;      // file size
   UINT64            m_file_max;       // file capacity
   //--- buffer
   char             *m_buffer;         // buffer
   UINT              m_buffer_size;    // buffer size

public:
                     CTextBase(void);
                    ~CTextBase(void);
   //--- open/close
   bool              Open(LPCWSTR path,const char delimiter='|');
   void              Close(void);
   //--- shutdown
   void              Shutdown(void);
   //--- access
   bool              IsOpen(void)  const { return(m_file.IsOpen()); }
   bool              IsEmpty(void) const { return(m_file_size==0);  }
   UINT64            ID(void)      const { return(m_id);            }
   //--- write
   bool              WriteLine(LPCWSTR text);
   bool              WriteRecord(LPCWSTR text);
   //--- flush
   bool              Flush(void) { return(m_file.Flush()); }

private:
   //--- initialization
   bool              Initialize(void);
   //--- raw write
   bool              Write(LPCSTR text);
   //--- reserve buffer
   bool              Reserve(const UINT size);
  };
//+------------------------------------------------------------------+

