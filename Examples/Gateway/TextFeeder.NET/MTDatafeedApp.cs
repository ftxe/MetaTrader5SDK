//+------------------------------------------------------------------+
//|                                          MetaTrader 5 TextFeeder |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
namespace TextFeeder.NET
  {
   using MetaQuotes.MT5CommonAPI;
   using MetaQuotes.MT5GatewayAPI;
   using System;
   using System.Threading;
   //+------------------------------------------------------------------+
   //| Server and external connection management class                  |
   //+------------------------------------------------------------------+
   class CMTDatafeedApp: CIMTGatewaySink
     {
      enum constants
        {
         SLEEP_TIMEOUT=500,
        };
      //--- gateway
      CIMTGatewayAPI    m_gateway;
      //--- datasource
      CDataSource       m_source;
      //--- sign of operation
      int               m_workflag;
      //+------------------------------------------------------------------+
      //| Constructor                                                      |
      //+------------------------------------------------------------------+
      public CMTDatafeedApp()
        {
         m_gateway =null;
         m_source  =null;
         m_workflag=0;
        }
      //+------------------------------------------------------------------+
      //| Destructor                                                       |
      //+------------------------------------------------------------------+
      public override void Release()
        {
         //--- complete the operation of all
         Shutdown();
         //---
         base.Release();
        }
      //+------------------------------------------------------------------+
      //| Datafeed additional information                                  |
      //+------------------------------------------------------------------+
      void Info(ref MTGatewayInfo info)
        {
         //--- form the info data
         info.version           =CTextFeeder.ProgramVersion;
         info.version_api       =SMTGatewayAPIFactory.GatewayAPIVersion;
         info.name_default      =CTextFeeder.ProgramName;
         info.copyright         =CTextFeeder.Copyright;
         info.server_default    ="localhost";
         info.login_default     ="";
         info.password_default  ="";
         info.parameters_default="TickPeriod=5000\\n"+
                                 "TickFile=_ticks.txt\\n"+
                                 "BookPeriod=5000\\n"+
                                 "BookFile=_books.txt\\n"+
                                 "NewsPeriod=10000\\n"+
                                 "NewsFile=_news.txt\\n"+
                                 "EventsPeriod=50000\\n"+
                                 "EventsFile=_events.txt\n";
         info.mode              =MTGatewayInfo.EnModes.GATEWAY_MODE_QUOTES|MTGatewayInfo.EnModes.GATEWAY_MODE_NEWS;
         info.fields            =MTGatewayInfo.EnFields.GATEWAY_FIELD_ALL;
         info.description       =CTextFeeder.ProgramDescription;
        }
      //+------------------------------------------------------------------+
      //| Initialization                                                   |
      //+------------------------------------------------------------------+
      public bool Initialize(string[] args)
        {
         MTGatewayInfo info;
         MTRetCode     ret=MTRetCode.MT_RET_OK;
         //--- checking
         if(args==null)
            return(false);
         //--- library: initialization
         if(SMTGatewayAPIFactory.Initialize()!=MTRetCode.MT_RET_OK)
            return(false);
         //--- fill MTGatewayInfo
         info=new MTGatewayInfo();
         Info(ref info);
         //--- gateway: creation
         m_gateway=SMTGatewayAPIFactory.CreateGateway(info,args,out ret);
         //--- checking
         if(ret!=MTRetCode.MT_RET_OK || m_gateway==null)
            return(false);
         //--- journal initialization
         ExtLogger.SetGateway(m_gateway);
         //---
         if((ret=RegisterSink())!=MTRetCode.MT_RET_OK)
           {   
            ExtLogger.Out(EnMTLogCode.MTLogErr,"can't register CIMTGatewaySink [{0}]",ret);
            return(false);
           }
         //--- datasource: creation
         m_source=new CDataSource();
         //--- everything is ok
         return(true);
        }
      //+------------------------------------------------------------------+
      //| Operation                                                        |
      //+------------------------------------------------------------------+
      public bool Run(string address=null)
        {
         bool started=false;
         //--- checking
         if(m_gateway==null || m_source==null)
            return(false);
         //--- start the processing of the external connection
         if(started=m_source.Start(m_gateway))
           {
            //--- start the receipt of incoming connections
            started=m_gateway.Start(this,address)==MTRetCode.MT_RET_OK;
           }
         //--- if it has started, set the operation flag
         if(started)
            Interlocked.Exchange(ref m_workflag,1);
         //--- the main loop of the external connection control
         while(Interlocked.Add(ref m_workflag,0)!=0)
           {
            //--- check the external connection
            if(!m_source.Check())
               break;
            //--- sleep
            Thread.Sleep((int)constants.SLEEP_TIMEOUT);
           }
         //--- stop the gateway
         m_gateway.Stop();
         //--- complete the operation of the datasource
         m_source.Shutdown();
         //--- everything is ok
         return(true);
        }
      //+------------------------------------------------------------------+
      //| Completing operation                                             |
      //+------------------------------------------------------------------+
      public void Shutdown()
        {
         //--- delete the datasource
         if(m_source!=null)
           {
            m_source.Dispose();
            m_source=null;
           }
         //--- refresh the journal
         ExtLogger.SetGateway(null);
         //--- release the gateway interface
         if(m_gateway!=null)
           {
            m_gateway.Release();
            m_gateway=null;
           }
         //--- complete the operation of the library
         SMTGatewayAPIFactory.Shutdown();
        }
      //+------------------------------------------------------------------+
      //| Getting the datasource description                               |
      //+------------------------------------------------------------------+
      public override void OnGatewayConfig(UInt64 login,CIMTConFeeder config)
        {
         //--- complete the operation
         if(m_source!=null)
            m_source.Init(config);
        }
      //+------------------------------------------------------------------+
      //| Completing the operation of the datafeed                         |
      //+------------------------------------------------------------------+
      public override void OnGatewayShutdown(UInt64 login)
        {
         //--- reset the operation flag
         Interlocked.Exchange(ref m_workflag,0);
        }
     }
  }
//+------------------------------------------------------------------+
