//+------------------------------------------------------------------+
//|                                         MetaTrader 5 NFA.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "TextBase.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CTextBase::CTextBase(void) : m_id(0),m_delimiter(0),
                             m_file_size(0),m_file_max(0),
                             m_buffer(NULL),m_buffer_size(0)
  {
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CTextBase::~CTextBase(void)
  {
   Shutdown();
  }
//+------------------------------------------------------------------+
//| Open                                                            |
//+------------------------------------------------------------------+
bool CTextBase::Open(LPCWSTR path,const char delimiter/*='|'*/)
  {
//--- close
   Close();
//--- checks
   if(!path)
      return(false);
   m_delimiter=delimiter;
//--- open file
   if(!m_file.Open(path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL))
     {
      Close();
      return(false);
     }
//--- initialize base
   if(!Initialize())
     {
      Close();
      return(false);
     }
//---
   return(true);
  }
//+------------------------------------------------------------------+
//| Close                                                            |
//+------------------------------------------------------------------+
void CTextBase::Close(void)
  {
//--- opened?
   if(m_file.IsOpen())
     {
      //--- cut file and close it
      m_file.ChangeSize(m_file_size);
      m_file.Close();
     }
//--- cleanup
   m_file_size=m_file_max=0;
   m_delimiter=0;
   m_id=0;
  }
//+------------------------------------------------------------------+
//| Shutdown                                                         |
//+------------------------------------------------------------------+
void CTextBase::Shutdown(void)
  {
//--- close
   Close();
//--- delete buffer
   if(m_buffer)
     {
      delete[] m_buffer;
      m_buffer=NULL;
     }
   m_buffer=0;
  }
//+------------------------------------------------------------------+
//| Raw write                                                        |
//+------------------------------------------------------------------+
bool CTextBase::Write(LPCSTR text)
  {
//--- checks
   if(!text || !m_file.IsOpen())
      return(false);
//--- string len
   UINT text_len=lstrlenA(text);
//--- check space in file
   if(!m_file_max || m_file_size+text_len>=m_file_max)
     {
      //--- flush
      m_file.Flush();
      //--- increase file size
      if(!m_file.ChangeSize(m_file_max+BASE_GROW_STEP))
        {
         Close();
         return(false);
        }
      //---
      m_file_max+=BASE_GROW_STEP;
      //--- seek to end of data
      if(m_file.Seek(m_file_size,FILE_BEGIN)!=m_file_size)
        {
         Close();
         return(false);
        }
     }
//--- write to file
   if(m_file.Write(text,text_len)!=text_len)
     {
      Close();
      return(false);
     }
//--- update file size
   m_file_size+=text_len;
//--- ok
   return(true);
  }
//+------------------------------------------------------------------+
//| Write string                                                     |
//+------------------------------------------------------------------+
bool CTextBase::WriteLine(LPCWSTR text)
  {
//--- check
   if(!text)
      return(false);
//--- reserve buffer
   if(!Reserve(CMTStr::Len(text)+sizeof("\r\n")))
      return(false);
//--- copy string to ansi buffer
   CMTStr::Copy(m_buffer,m_buffer_size,text);
   strcat_s(m_buffer,m_buffer_size,"\r\n");
//--- raw write
   return(Write(m_buffer));
  }
//+------------------------------------------------------------------+
//| Write record (with id in beginning)                              |
//+------------------------------------------------------------------+
bool CTextBase::WriteRecord(LPCWSTR text)
  {
   char id[64];
//--- check
   if(!text)
      return(false);
//--- prepare id
   UINT idlen=sprintf_s(id,sizeof(id),"%I64u%c",m_id+1,m_delimiter);
//--- reserve buffer
   if(!Reserve(idlen+CMTStr::Len(text)+sizeof("\r\n")))
      return(false);
//--- write ansi string
   strncpy_s(m_buffer,m_buffer_size,id, idlen);
   CMTStr::Copy(m_buffer+idlen,m_buffer_size-idlen,text);
   strcat_s(m_buffer,m_buffer_size,"\r\n");
//--- raw write
   if(Write(m_buffer))
     {
      m_id++;
      return(true);
     }
//--- fail
   return(false);
  }
//+------------------------------------------------------------------+
//| Initialization                                                   |
//+------------------------------------------------------------------+
bool CTextBase::Initialize(void)
  {
   char buffer[BASE_READ_SIZE],idbuf[BADE_IDEN_SIZE]={0};
   UINT readed,i,id,idsize=0;
   bool newline=true;
//--- update file capacity
   m_file_max=m_file.Size();
//--- read buffer
   while((readed=m_file.Read(buffer,sizeof(buffer)))>0)
     {
      //--- scan buffer
      for(i=0;i<readed;i++,m_file_size++)
        {
         //--- eof?
         if(buffer[i]=='\0')
            break;
         //--- is not new line?
         if(!newline)
           {
            //--- new line?
            if(buffer[i]=='\n')
               newline=true;
            //--- check id buffer?
            if(idsize)
              {
               //--- parse
               id    =atoi(idbuf);
               m_id  =max(m_id,id);
               idsize=0;
              }
            continue;
           }
         //--- digit?
         if(isdigit(buffer[i]))
           {
            //--- collect id
            if(idsize<sizeof(idbuf)-1)
               idbuf[idsize++]=buffer[i];
            continue;
           }
         //--- reset new line
         newline=false;
        }
     }
//--- seek to data end
   if(m_file.Seek(m_file_size,FILE_BEGIN)!=m_file_size)
      return(false);
//--- append new line if needed
   if(!newline || idsize)
      return(Write("\r\n"));
//--- ok
   return(true);
  }
//+------------------------------------------------------------------+
//| Reserve buffer                                                   |
//+------------------------------------------------------------------+
bool CTextBase::Reserve(const UINT size)
  {
//--- check
   if(!size)
      return(false);
//--- check size
   if(!m_buffer || size>m_buffer_size)
     {
      if(m_buffer)
         delete[] m_buffer;
      //--- allocate new buffer
      if((m_buffer=new(std::nothrow) char[size])==NULL) return(false);
      //--- setup size
      m_buffer_size=size;
      //--- clear buffer
      ZeroMemory(m_buffer,m_buffer_size);
     }
//--- ok
   return(true);
  }
//+------------------------------------------------------------------+
