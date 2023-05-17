//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "Zip\UnzipFile.h"
//+------------------------------------------------------------------+
//| Class helper for reading/writing ANSI-encoded files line-by-line |
//+------------------------------------------------------------------+
class CTextReader
  {
   enum constants
     {
      BUFFER_SIZE=256*1024                 // 256 Kb
     };
private:
   CMTFile           m_file;
   CUnzipFile        m_file_zip;
   char             *m_buf;               // file buffer
   char             *m_str;               // buffer for the string reading
   char             *m_str_end;           // the end of the string
   UINT              m_buf_size;          // sizes of buffers
   UINT              m_index;             // index of current symbol
   UINT              m_readed;            // number of symbols readed
   //--- line parser
   CMTStr4096        m_line;
   int               m_line_pos;

public:
                     CTextReader(const UINT buf_size=BUFFER_SIZE);
                    ~CTextReader(void);
   //--- open/close
   bool              Open(LPCWSTR path);
   void              Close(void);
   //--- read line
   bool              ReadLine(void);
   //--- line parser
   void              String(LPWSTR dst,UINT dstsize);
   template <UINT dstsize>
   void              String(wchar_t (&dst)[dstsize]) { String(dst,dstsize); }
   void              Date(INT64 &value);
   void              DateTime(INT64 &value);
   void              Double(double &value);
   void              Integer(UINT64 &value);
   void              Skip(const UINT count=1);

private:
   //--- line parse
   bool              LineEnd(void) const;
   void              LineNextColumn(void);
   //--- reading data from file
   UINT              Read(void *buffer,const UINT length);
   //--- processing string
   bool              ReadNextLine(void);
  };
//+------------------------------------------------------------------+
