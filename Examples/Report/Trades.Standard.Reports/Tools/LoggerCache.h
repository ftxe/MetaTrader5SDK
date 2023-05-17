//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Logger request result class                                      |
//+------------------------------------------------------------------+
class CLoggerResult
  {
private:
   INT64             m_to;                // request end time
   INT64             m_to_result;         // request result end time
   MTLogRecord      *m_logs;              // first log record
   UINT              m_logs_total;        // log record count

public:
   //--- timeout log record
   static const wchar_t s_timeout_record[];

public:
   //--- constructor/destructor
                     CLoggerResult(void);
                    ~CLoggerResult(void);
   //--- clear contents
   void              Clear(IMTReportAPI &api);
   //--- logger request
   MTAPIRES          Request(IMTReportAPI &api,const INT64 from,const INT64 to,const CMTStr64 &request);
   //--- request result end time
   INT64             ResultTo(void) const          { return(m_to_result); }
   //--- check for completenes
   bool              Complete(void) const          { return(m_to==m_to_result); }
   //--- log record by index
   const MTLogRecord* Record(const UINT pos) const { return(m_logs && pos<m_logs_total ? m_logs+pos : nullptr); }
  };
//+------------------------------------------------------------------+
//| Logger request result class array type                           |
//+------------------------------------------------------------------+
typedef TMTArray<CLoggerResult> CLoggerResultArray;
//+------------------------------------------------------------------+
//| Logger cache class                                               |
//+------------------------------------------------------------------+
class CLoggerCache
  {
private:
   IMTReportAPI     *m_api;               // api interface
   CMTStr64          m_request;           // logger request string
   INT64             m_ctm_from;          // request begin time
   CLoggerResultArray m_logs;             // request results

public:
   //--- constructor/destructor
   explicit          CLoggerCache(IMTReportAPI *api);
                    ~CLoggerCache(void);
   //--- clear contents
   void              Clear(void);
   //--- add id to request
   bool              Add(const UINT64 id,const INT64 ctm);
   bool              AddPair(const UINT64 id1,const INT64 ctm1,const UINT64 id2,const INT64 ctm2);
   //--- logger request
   MTAPIRES          Request(void);
   //--- write log records to html
   MTAPIRES          HtmlWriteLog(const UINT64 id1,const UINT64 id2=0) const;

private:
   //--- clear results
   void              ClearResult(void);
   //--- add request string
   bool              AddRequest(const CMTStr &request);
   //--- update request time
   void              UpdateTime(const INT64 ctm);
  };
//+------------------------------------------------------------------+
//| Logger cache holder class                                        |
//+------------------------------------------------------------------+
class CLoggerCacheHolder
  {
private:
   IMTReportAPI     *m_api;               // api interface
   CLoggerCache      m_cache;             // logger cache
   UINT              m_pos_beg;           // cached item index begin
   UINT              m_pos_end;           // cached item index end

public:
   //--- constructor/destructor
   explicit          CLoggerCacheHolder(IMTReportAPI *api);
                    ~CLoggerCacheHolder(void);
   //--- cache array items
   const IMTPosition* CachePositions(const IMTPositionArray &positions,const UINT pos,MTAPIRES &res);
   const IMTOrder*   CacheOrders(const IMTOrderArray &orders,const UINT pos,MTAPIRES &res);
   const IMTDeal*    CacheDeals(const IMTDealArray &deals,const UINT pos,MTAPIRES &res);
   //--- write item log to html
   MTAPIRES          HtmlWriteLogPosition(const IMTPosition &position);
   MTAPIRES          HtmlWriteLogOrder(const IMTOrder &order);
   MTAPIRES          HtmlWriteLogDeal(const IMTDeal &deal);

private:
   //--- check cache for item index
   bool              CheckCache(const UINT pos);
   //--- add item to request
   bool              AddPosition(const IMTPosition &position);
   bool              AddOrder(const IMTOrder &order);
   bool              AddDeal(const IMTDeal &deal);
  };
//+------------------------------------------------------------------+
