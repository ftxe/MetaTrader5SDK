//+------------------------------------------------------------------+
//|                            MetaTrader 5 Capital.Standard.Reports |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "CurrencyReport.h"
#include "FtdTotalSection.h"
#include "..\Cache\ClientFtdCache.h"
//+------------------------------------------------------------------+
//| FTD by section Report                                            |
//+------------------------------------------------------------------+
class CFtdTotalSectionReport : public CCurrencyReport, public IClientFtdReceiver
  {
private:
   CClientFtdCache   m_client_ftd;                 // client FTD cache
   CFtdTotalSection  m_section_lead_source;        // FTD by lead source
   CFtdTotalSection  m_section_lead_campaign;      // FTD by lead campaign
   CFtdTotalSection  m_section_country;            // FTD by country
   CFtdTotalSection  m_section_value;              // FTD by value
   //--- static data
   static MTReportInfo s_info;                     // static report info

public:
   //--- constructor/destructor
                     CFtdTotalSectionReport(void);
   virtual          ~CFtdTotalSectionReport(void);
   //--- report information
   static void       Info(MTReportInfo &info);
   //--- fabric method
   static IMTReportContext* Create(void);

private:
   //--- clear
   virtual void      Clear(void) override;
   //--- get report parameters and create caches
   virtual MTAPIRES  Prepare(void) override;
   //--- request data from server and compute statistics
   virtual MTAPIRES  Calculate(void) override;
   //--- calculate top
   MTAPIRES          CalculateTop(void);
   //--- client FTD read handler
   virtual MTAPIRES  ClientFtdRead(const CClientCache::ClientCache &client,const INT64 time,const double value) override;
   //--- prepare all graphs to be shown
   virtual MTAPIRES  PrepareGraphs(void) override;
  };
//+------------------------------------------------------------------+
