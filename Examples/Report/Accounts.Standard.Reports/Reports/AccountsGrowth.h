//+------------------------------------------------------------------+
//|                           MetaTrader 5 Accounts.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "..\Tools\ReportBase.h"
#include "..\Tools\UserSelect.h"
//+------------------------------------------------------------------+
//| Accounts Grow Report                                             |
//+------------------------------------------------------------------+
class CAccountsGrowth : public CReportBase
  {
private:
   //--- constants
   enum constants
     {
      PERIOD_YEARS            =5,               // report period (years)
      PERIOD_MONTHS           =PERIOD_YEARS*12, // report period (months)
      PERIOD_INACTIVE_MONTHS  =3,               // report inactive account period (months)
     };

private:
   #pragma pack(push,1)
   //--- user record
   struct UserRecord
     {
      UINT64            login;            // login
      INT64             registration;     // registration time
      INT64             last_access;      // last access time
     };
   //--- grow dataset record
   struct GrowRecord
     {
      INT64             date;                   // login
      UINT32            real;                   // real accounts
      UINT32            preliminary;            // preliminary accounts
      UINT32            demo;                   // demo accounts
      UINT32            contest;                // contest accounts
     };
   //--- month dataset record
   struct MonthRecord
     {
      wchar_t           month[4];                     // month
      UINT32            real [PERIOD_YEARS];          // real accounts
      UINT32            preliminary [PERIOD_YEARS];   // preliminary accounts
      UINT32            demo [PERIOD_YEARS];          // demo accounts
      UINT32            contest [PERIOD_YEARS];       // contest accounts
     };
   //--- active dataset record
   struct ActiveRecord
     {
      wchar_t           type[16];               // account type
      UINT32            active;                 // active accounts
     };
   #pragma pack(pop)

private:
   //--- current time
   INT64             m_ctm;
   tm                m_ctm_ttm;
   INT64             m_checktime;               // activity check time
   //--- month indexes
   UINT              m_month_data;              // start month with data
   UINT              m_month_firstyear;         // start month for first year
   //--- real accounts
   UINT              m_real[PERIOD_MONTHS];
   UINT              m_real_active;
   UINT              m_real_total;
   //--- preliminary accounts
   UINT              m_preliminary[PERIOD_MONTHS];
   UINT              m_preliminary_active;
   UINT              m_preliminary_total;
   //--- demo accounts
   UINT              m_demo[PERIOD_MONTHS];
   UINT              m_demo_active;
   UINT              m_demo_total;
   //--- contest accounts
   UINT              m_contest[PERIOD_MONTHS];
   UINT              m_contest_active;
   UINT              m_contest_total;
   //--- group & user
   IMTConGroup      *m_group;                      // group interface
   CUserSelect       m_user_select;                // user select object
   //--- charts
   IMTReportChart   *m_chart_grow;
   IMTReportChart   *m_chart_real;
   IMTReportChart   *m_chart_preliminary;
   IMTReportChart   *m_chart_demo;
   IMTReportChart   *m_chart_contest;
   IMTReportChart   *m_chart_active;
   //---
   static const MTReportInfo s_info;               // report information     
   static const ReportColumn s_columns_grow[];     // column descriptions Grow
   static const ReportColumn s_column_month;       // column description Month
   static const ReportColumn s_column_year;        // column description Year
   static const ReportColumn s_columns_active[];   // column descriptions Active
   static const DatasetField s_user_fields[];      // user request fields descriptions
   static const UINT s_request_limit=1000000;      // request limit

public:
   //--- constructor/destructor
                     CAccountsGrowth(void);
   virtual          ~CAccountsGrowth(void);
   //--- get information about report
   static void       Info(MTReportInfo& info);

private:
   //--- base overrides
   virtual void      Clear(void) override;
   virtual MTAPIRES  Prepare(void) override;
   virtual MTAPIRES  Write(UINT type) override;
   //--- report generation HTML
   MTAPIRES          GenerateHtml(void);
   //--- report generation dashboard
   MTAPIRES          GenerateDashboard(void);
   //--- prepare data
   MTAPIRES          PrepareData(void);
   MTAPIRES          PrepareGroup(LPCWSTR group);
   MTAPIRES          PrepareGroupPart(const UINT type,IMTDataset &users,UINT64 &login_last);
   //--- prepare charts
   MTAPIRES          PrepareChartGrow(void);
   MTAPIRES          PrepareChartReal(void);
   MTAPIRES          PrepareChartPreliminary(void);
   MTAPIRES          PrepareChartDemo(void);
   MTAPIRES          PrepareChartContest(void);
   MTAPIRES          PrepareChartActive(void);
   IMTReportSeries*  PrepareMonthTitleSeries(void);
   //--- prepare datasets
   MTAPIRES          PrepareDataColumns(IMTDataset *data,const ReportColumn *columns,const UINT total);
   MTAPIRES          PrepareDataGrow(IMTDataset *data);
   MTAPIRES          PrepareDataMonth(IMTDataset *data);
   MTAPIRES          PrepareDataActive(IMTDataset *data);
   //--- prepare widget
   MTAPIRES          PrepareWidget(IMTReportAPI *api,IMTDataset *data,UINT column,UINT column_total,LPCWSTR title,const UINT type);
   //--- month management
   bool              TimeToMonthIndex(const INT64 ctm,UINT &index);
   bool              MonthIndexToStr(const UINT index,CMTStr& str);
   INT64             MonthIndexToTime(const UINT index);
   //--- colors
   UINT              GetColorForYear(const UINT index);
  };
//+------------------------------------------------------------------+
