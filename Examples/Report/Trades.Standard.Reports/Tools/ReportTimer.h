//+------------------------------------------------------------------+
//|                             MetaTrader 5 Trades.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Report timer helper class                                        |
//+------------------------------------------------------------------+
class CReportTimer
  {
private:
   UINT              m_start;        // start time in ms

public:
                     CReportTimer(void) : m_start(timeGetTime()) {}
   //--- elapsed time in ms
   UINT              Elapsed(void) const  { return(timeGetTime()-m_start); }
  };
//+------------------------------------------------------------------+
