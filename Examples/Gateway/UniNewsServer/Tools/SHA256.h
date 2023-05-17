//+------------------------------------------------------------------+
//|                               MetaTrader 5 Universal News Server |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| SHA-256 realization                                              |
//| https://en.wikipedia.org/wiki/SHA-2                              |
//+------------------------------------------------------------------+
class CSHA256
  {
private:
   enum constants
     {
      MAX_HASH=32,                  // maximum hash size
     };
   //--- state structure
   struct ShaState
     {
      ULONG             state[8];   // state
      ULONG             length;     // total size
      ULONG             curlen;     // current size
      UCHAR             buf[64];    // data buffer
     };

private:
   static const ULONG s_keys[64];

private:
   ShaState          m_state;

public:
   //--- constructor/destructor
                     CSHA256(void);
                    ~CSHA256(void);
   //--- hash calculation
   static void       CalculateHash(const PUCHAR buf,const UINT len,PUCHAR hash,const UINT hash_len);

private:
   void              Compress(void);
   void              Init(void);
   void              Process(UCHAR const *buf,const UINT len);
   void              Done(PUCHAR hash,const UINT hash_len);
  };
//+------------------------------------------------------------------+
