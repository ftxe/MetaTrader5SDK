//+------------------------------------------------------------------+
//|                               MetaTrader 5 Daily.Standard.Report |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Daily Trade Report                                               |
//+------------------------------------------------------------------+
class CDailyTradeReport : public IMTReportContext
  {
private:
   //--- daily deals info
   struct DailyDealsInfo
     {
      double            profit;       // daily profit
      double            loss;         // daily loss
      INT64             date;         // day
      UINT              profit_count; // number of profit deals
      UINT              loss_count;   // number of loss deals
     };
   //--- positions info
   struct PositionInfo
     {
      double            profit;       // profit/loss of position
      UINT64            position;     // position id
     };
   //--- deals info
   struct DealInfo
     {
      UINT64            login;        // user
      double            profit;       // profit/loss of deal
      UINT64            ticket;       // deal
     };
   //--- account info
   struct AccountInfo
     {
      UINT64            login;        // user
      double            closed_pl;    // closed profit/loss
      UINT              total_deals;  // number of trades
     };
   //--- arrays
   typedef TMTArray<DailyDealsInfo> DailyDealsArray;
   typedef TMTArray<DealInfo,8192> DealsArray;
   typedef TMTArray<PositionInfo,8192> PositionArray;
   typedef TMTArray<AccountInfo,2048> AccountArray;
   //--- constants
   enum constants
     {
      MAX_TOP_LIST      =10,
      CURRENCY_DIGITS   =2,
      DAILY_DEALS_PERIOD=SECONDS_IN_WEEK*2,  // period for graph
     };
   //--- dataset records
#pragma pack(push,1)
   //--- daily record
   struct DailyRecord
     {
      INT64             date;
      double            profit;
      double            loss;
      double            profit_loss;
      UINT              profit_count;
      UINT              loss_count;
     };
   //--- Total record
   struct TotalRecord
     {
      wchar_t           type[32];
      double            value;
     };
   //--- Deal record
   struct DealRecord
     {
      UINT64            deal;
      UINT64            login;
      wchar_t           name[128];
      wchar_t           symbol[32];
      wchar_t           group[64];
      UINT              type;
      UINT64            volume;
      double            price;
      double            swap;
      double            profit;
     };
   //--- Account record
   struct AccountRecord
     {
      UINT64            login;
      wchar_t           name[128];
      wchar_t           group[64];
      UINT              leverage;
      UINT              placed_orders;
      UINT              orders;
      UINT              deals;
      double            balance;
      double            floating_pl;
      double            closed_pl;
     };
   //--- Position record
   struct PositionRecord
     {
      UINT64            login;
      UINT64            ticket;
      wchar_t           name[128];
      wchar_t           symbol[32];
      wchar_t           group[64];
      UINT              type;
      UINT64            volume;
      double            open_price;
      double            sl;
      double            tp;
      double            market_price;
      double            swap;
      INT               points;
      double            profit;
     };
#pragma pack(pop)

private:
   IMTReportAPI*     m_api;                  // api interface
   //--- common configs
   IMTConParam*      m_currency;             // currency for converting
   UINT              m_digits;               // currency digits
   IMTUser*          m_user;                 // user 
   IMTConGroup*      m_group;                // group`s config
   IMTPositionArray* m_positions;            // positions array
   IMTOrderArray*    m_orders;               // positions array
   //--- charts
   IMTReportChart*   m_chart_profit;         // profit/loss deals chart
   IMTReportChart*   m_chart_number;         // number of deals chart
   IMTReportChart*   m_chart_position;       // position pie chart
   //--- daily datas
   DailyDealsArray   m_daily_deals;          // array of all daily deals for period
   //--- positions processing
   PositionArray     m_positions_loss;       // array of loss positions
   PositionArray     m_positions_profit;     // array of profit positions
   IMTPosition*      m_position;             // position
   double            m_postion_total_loss;   // total loss from all positions
   double            m_postion_total_profit; // total profit from all positions
   //--- deals processing
   DealsArray        m_deals_loss;           // all loss deals in certain day
   DealsArray        m_deals_profit;         // all profit deals in certain day
   IMTDeal*          m_deal;                 // deal
   //--- accounts processing
   AccountArray      m_accounts;             // all profit accounts in certain day
   //--- static data
   static MTReportInfo s_info;               // report information
   static ReportColumn s_columns_daily[];    // column description daily deal
   static ReportColumn s_columns_total[];    // column description total
   static ReportColumn s_columns_deal[];     // column description deal
   static ReportColumn s_columns_account[];  // column description account
   static ReportColumn s_columns_position[]; // column description position

public:
   //--- constructor/destructor
                     CDailyTradeReport(void);
   virtual          ~CDailyTradeReport(void);
   //--- get information about report
   static void       Info(MTReportInfo& info);
   //--- release plug-in
   virtual void      Release(void);
   //--- report generation
   virtual MTAPIRES  Generate(const UINT type,IMTReportAPI *api);

private:
   //--- create/release interfaces
   void              Clear(void);
   bool              CreateInterfaces(void);
   //--- get currency
   MTAPIRES          GetCurency(void);
   //--- load info
   MTAPIRES          LoadInfo(void);
   MTAPIRES          LoadPositions(void);
   MTAPIRES          LoadDeals(void);
   MTAPIRES          ComputeAccounts(void);
   MTAPIRES          ComputeDailyDeals(void);
   //--- convert money
   MTAPIRES          ConvertMoney(const double profit,const UINT64 login,double& converted);
   //--- prepare chart
   MTAPIRES          PrepareProfitLossChart(void);
   MTAPIRES          PrepareNumberChart(void);
   MTAPIRES          PreparePositionsChart(void);
   //--- write all html
   MTAPIRES          WriteAll(void);
   bool              WriteTableDeals(MTAPISTR& tag,MTAPIRES& res,const UINT counter,bool profit);
   bool              WriteTablePositions(MTAPISTR& tag,MTAPIRES& res,const UINT counter,bool profit);
   bool              WriteTopTrades(MTAPISTR& tag,MTAPIRES& retcode,UINT& counter);
   bool              WriteTableAccounts(MTAPISTR& tag,MTAPIRES& res,const UINT counter,bool profit);
   bool              WriteTopPosition(MTAPISTR& tag,MTAPIRES& retcode,UINT& counter);
   //--- generate dashboard report
   MTAPIRES          GenerateDashboard(void);
   //--- generate dashboard chart daily data
   IMTDataset*       ChartDailyData(MTAPIRES &res);
   //--- generate dashboard chart total
   MTAPIRES          AddChartTotal(void);
   //--- generate dashboard table deals
   MTAPIRES          AddTableDeal(LPCWSTR title,const DealsArray &deals);
   //--- generate dashboard table accounts
   MTAPIRES          AddTableAccount(void);
   //--- generate dashboard table positions
   MTAPIRES          AddTablePositions(LPCWSTR title,const PositionArray &positions);
   //--- add chart
   MTAPIRES          AddDashboardChart(IMTDataset *data,LPCWSTR title,bool bar,bool acc,UINT column_id,UINT column_count);
   //--- append currency to string
   void              AppendCurrency(CMTStr &str) const;
   //--- sorting
   static int        SortByProfitPosition(const void *left,const void *right);
   static int        SortByLossDeal(const void *left,const void *right);
   static int        SortByClosedPLAccount(const void *left, const void *right);
   static int        SortByProfitDeal(const void *left,const void *right);
   static int        SortByLoginDeal(const void *left, const void *right);
   static int        SortByLossPosition(const void *left, const void *right);
  };
//+------------------------------------------------------------------+

