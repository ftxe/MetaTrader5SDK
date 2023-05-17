//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Report interval class                                            |
//+------------------------------------------------------------------+
class CReportInterval
  {
public:
   //--- intervals for interpolation
   enum EnInterval
     {
      INTERVAL_HOUR  =0,                              // hourly
      INTERVAL_DAY   =1,                              // daily
      INTERVAL_WEEK  =2,                              // weekly
      INTERVAL_MONTH =3,                              // monthly
      INTERVAL_COUNT =INTERVAL_MONTH                  // count of enumerable
     };

private:
   const UINT        m_days_week;                     // number of days for week interval
   const UINT        m_days_months;                   // number of days for month interval
   const UINT        m_days_hour;                     // number of days for hour interval
   INT64             m_from;                          // report time range begin
   INT64             m_to;                            // report time range end
   INT64             m_from_begin;                    // report time range begin period begin
   tm                m_from_tm;                       // report time range begin parsed time
   UINT              m_from_month;                    // report first month index for month interval
   UINT              m_week_from;                     // report weeks range begin
   UINT              m_week_to;                       // report weeks range end
   UINT              m_period;                        // interval period in seconds
   EnInterval        m_interval;                      // interval

public:
                     CReportInterval(const UINT days_week,const UINT days_months,const UINT days_hour=0);
                    ~CReportInterval(void);
   //--- initialization
   MTAPIRES          Initialize(IMTReportAPI *api);
   //--- clear
   void              Clear(void);
   //--- report time range
   INT64             From(void) const                    { return(m_from); }
   INT64             To(void) const                      { return(m_to); }
   //--- check time in range   
   bool              Contains(const INT64 ctm) const     { return(m_from<=ctm && ctm<=m_to); }
   //--- check weeek in range   
   bool              WeekContains(const UINT week) const { return(m_week_from<=week && week<=m_week_to); }
   //--- time period total count
   UINT              PeriodTotal(void) const;
   //--- time period index
   int               PeriodIndex(INT64 ctm) const;
   //--- time period start date by index
   INT64             PeriodDate(const UINT pos) const;
   //--- time month period start date by index
   void              PeriodDateMonth(tm &ttm,const UINT pos) const;
   //--- format period
   const CMTStr&     FormatPeriod(CMTStr &str,const UINT pos) const;
   //--- month index
   static UINT       MonthIndex(const INT64 ctm);
   //--- monday week begin
   static INT64      WeekBeginMonday(const INT64 ctm)    { return(SMTTime::WeekBegin(ctm-SECONDS_IN_DAY)+SECONDS_IN_DAY); }
   //--- monday week number
   static UINT       WeekMonday(const INT64 ctm)         { return(UINT((ctm+3*SECONDS_IN_DAY)/SECONDS_IN_WEEK)); }
  };
//+------------------------------------------------------------------+
