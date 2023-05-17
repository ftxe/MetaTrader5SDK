//+------------------------------------------------------------------+
//|                               MetaTrader 5 Daily.Standard.Report |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "ReportLog.h"
//+------------------------------------------------------------------+
//| Report base class                                                |
//+------------------------------------------------------------------+
class CReportBase : public IMTReportContext
  {
protected:
   const UINT        m_types;                         // report types mask
   IMTReportAPI     *m_api;                           // report api
   CReportLog        m_log;                           // logger

public:
                     CReportBase(const MTReportInfo &info);
   virtual          ~CReportBase(void);
   //--- release
   virtual void      Release(void);
   //--- report generation
   virtual MTAPIRES  Generate(const UINT type,IMTReportAPI *api);

protected:
   //--- clear overridable
   virtual void      Clear(void)=0;
   //--- report prepare overridable
   virtual MTAPIRES  Prepare(void)=0;
   //--- report write overridable
   virtual MTAPIRES  Write(void)=0;

private:
   //--- inner and outer clear
   void              ClearAll(void);
   //--- inner clear
   void              ClearInner(void);
  };
//+------------------------------------------------------------------+
