﻿//+------------------------------------------------------------------+
//|                                             MetaTrader 5 Web API |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
using System;
using System.Collections.Generic;
using System.Web.Script.Serialization;
using MetaQuotes.MT5WebAPI.Common.Utils;
//---
namespace MetaQuotes.MT5WebAPI.Common.Protocol
  {
  class MTTickBase : MTAPIBase
    {
    public MTTickBase(MTAsyncConnect connect) : base(connect) {}
    /// <summary>
    /// Get last ticks
    /// </summary>
    /// <param name="symbol">symbol name</param>
    /// <param name="ticks">list of tick</param>
    public MTRetCode TickLast(string symbol,out List<MTTick> ticks)
      {
      ticks = null;
      //--- send request
      Dictionary<string,string> data = new Dictionary<string,string> { { MTProtocolConsts.WEB_PARAM_SYMBOL,symbol } };
      //--- get answer
      byte[] answer;
      //--- send request
      if((answer = Send(MTProtocolConsts.WEB_CMD_TICK_LAST,data)) == null)
        {
        MTLog.Write(MTLogType.Error,"send ticks last failed");
        return MTRetCode.MT_RET_ERR_NETWORK;
        }
      //---
      string answerStr = MTUtils.GetString(answer);
      if(MTLog.IsWriteDebugLog) MTLog.Write(MTLogType.Debug,string.Format("result answer: {0}",answerStr));
      //--- parse answer
      MTRetCode errorCode;
      if((errorCode = ParseTickLast(MTProtocolConsts.WEB_CMD_TICK_LAST,answerStr,out ticks)) != MTRetCode.MT_RET_OK)
        {
        MTLog.Write(MTLogType.Error,string.Format("parse ticks last failed: {0}",MTFormat.GetErrorStandart(errorCode)));
        return errorCode;
        }
      //---
      return MTRetCode.MT_RET_OK;
      }
    /// <summary>
    /// check answer from MetaTrader 5 server
    /// </summary>
    /// <param name="command">command send</param>
    /// <param name="answer">answer from MT5 sever</param>
    /// <param name="ticks">result pasing</param>
    private static MTRetCode ParseTickLast(string command,string answer,out List<MTTick> ticks)
      {
      ticks = null;
      int pos = 0;
      //--- get command answer
      string commandReal = MTParseProtocol.GetCommand(answer,ref pos);
      if(command != commandReal)
        {
        MTLog.Write(MTLogType.Error,string.Format("answer command '{0}' is incorrect, wait {1}",command,commandReal));
        return MTRetCode.MT_RET_ERR_DATA;
        }
      //---
      MTTicksAnswer ticksAnswer = new MTTicksAnswer();
      //--- get param
      int posEnd = -1;
      MTAnswerParam param;
      while((param = MTParseProtocol.GetNextParam(answer,ref pos,ref posEnd)) != null)
        {
        switch(param.Name)
          {
          case MTProtocolConsts.WEB_PARAM_RETCODE:
          ticksAnswer.RetCode = param.Value;
          break;
          //---
          case MTProtocolConsts.WEB_PARAM_TRANS_ID:
          ticksAnswer.TransId = param.Value;
          break;
          }
        }
      //---
      MTRetCode errorCode;
      //--- check ret code
      if((errorCode = MTParseProtocol.GetRetCode(ticksAnswer.RetCode)) != MTRetCode.MT_RET_OK) return errorCode;
      //--- get json
      if((ticksAnswer.ConfigJson = MTParseProtocol.GetJson(answer,posEnd)) == null) return MTRetCode.MT_RET_REPORT_NODATA;
      //--- parsing Json
      ticks = ticksAnswer.GetFromJson();
      //--- parsing empty
      if(ticks == null) return MTRetCode.MT_RET_REPORT_NODATA;
      //---
      return MTRetCode.MT_RET_OK;
      }
    /// <summary>
    /// Get last tickets by symbol and group
    /// </summary>
    /// <param name="symbol">symbol name</param>
    /// <param name="group">group name</param>
    /// <param name="ticks">list of ticks</param>
    public MTRetCode TickLastGroup(string symbol,string group,out List<MTTick> ticks)
      {
      ticks = null;
      //--- send request
      Dictionary<string,string> data = new Dictionary<string,string>();
      data.Add(MTProtocolConsts.WEB_PARAM_SYMBOL, symbol);
      data.Add(MTProtocolConsts.WEB_PARAM_GROUP, group);
      //--- get answer
      byte[] answer;//--- send request
      if((answer = Send(MTProtocolConsts.WEB_CMD_TICK_LAST_GROUP,data)) == null)
        {
        MTLog.Write(MTLogType.Error,"send ticks last failed");
        return MTRetCode.MT_RET_ERR_NETWORK;
        }
      //---
      string answerStr = MTUtils.GetString(answer);
      if(MTLog.IsWriteDebugLog) MTLog.Write(MTLogType.Debug,string.Format("result answer: {0}",answerStr));
      //--- parse answer
      MTRetCode errorCode;
      if((errorCode = ParseTickLast(MTProtocolConsts.WEB_CMD_TICK_LAST_GROUP,answerStr,out ticks)) != MTRetCode.MT_RET_OK)
        {
        MTLog.Write(MTLogType.Error,string.Format("parse ticks group last failed: {0}",MTFormat.GetErrorStandart(errorCode)));
        return errorCode;
        }
      //---
      return MTRetCode.MT_RET_OK;
      }
    /// <summary>
    /// Get tick state
    /// </summary>
    /// <param name="symbol">symbol name</param>
    /// <param name="ticksStat">list of tick state</param>
    public MTRetCode TickStat(string symbol,out List<MTTickStat> ticksStat)
      {
      ticksStat = null;
      //--- send request
      Dictionary<string,string> data = new Dictionary<string,string> { { MTProtocolConsts.WEB_PARAM_SYMBOL,symbol } };
      //--- get answer
      byte[] answer;
      //--- send request
      if((answer = Send(MTProtocolConsts.WEB_CMD_TICK_STAT,data)) == null)
        {
        MTLog.Write(MTLogType.Error,"send tick stat failed");
        return MTRetCode.MT_RET_ERR_NETWORK;
        }
      //---
      string answerStr = MTUtils.GetString(answer);
      if(MTLog.IsWriteDebugLog) MTLog.Write(MTLogType.Debug,string.Format("result answer: {0}",answerStr));
      //--- parse answer
      MTRetCode errorCode;
      if((errorCode = ParseTickStat(MTProtocolConsts.WEB_CMD_TICK_STAT,answerStr,out ticksStat)) != MTRetCode.MT_RET_OK)
        {
        MTLog.Write(MTLogType.Error,string.Format("parse tick stat failed: {0}",MTFormat.GetErrorStandart(errorCode)));
        return errorCode;
        }
      //---
      return MTRetCode.MT_RET_OK;
      }
    /// <summary>
    /// check answer from MetaTrader 5 server
    /// </summary>
    /// <param name="command">command send</param>
    /// <param name="answer">answer from MT5 sever</param>
    /// <param name="ticksStat">result pasing</param>
    private static MTRetCode ParseTickStat(string command,string answer,out List<MTTickStat> ticksStat)
      {
      ticksStat = null;
      int pos = 0;
      //--- get command answer
      string commandReal = MTParseProtocol.GetCommand(answer,ref pos);
      if(command != commandReal)
        {
        MTLog.Write(MTLogType.Error,string.Format("answer command '{0}' is incorrect, wait {1}",command,commandReal));
        return MTRetCode.MT_RET_ERR_DATA;
        }
      //---
      MTTickStatAnswer ticksAnswer = new MTTickStatAnswer();
      //--- get param
      int posEnd = -1;
      MTAnswerParam param;
      while((param = MTParseProtocol.GetNextParam(answer,ref pos,ref posEnd)) != null)
        {
        switch(param.Name)
          {
          case MTProtocolConsts.WEB_PARAM_RETCODE:
          ticksAnswer.RetCode = param.Value;
          break;
          //---
          case MTProtocolConsts.WEB_PARAM_TRANS_ID:
          ticksAnswer.TransId = param.Value;
          break;
          }
        }
      //---
      MTRetCode errorCode;
      //--- check ret code
      if((errorCode = MTParseProtocol.GetRetCode(ticksAnswer.RetCode)) != MTRetCode.MT_RET_OK) return errorCode;
      //--- get json
      if((ticksAnswer.ConfigJson = MTParseProtocol.GetJson(answer,posEnd)) == null) return MTRetCode.MT_RET_REPORT_NODATA;
      //--- parsing Json
      ticksStat = ticksAnswer.GetFromJson();
      //--- parsing empty
      if(ticksStat == null) return MTRetCode.MT_RET_REPORT_NODATA;
      //---
      return MTRetCode.MT_RET_OK;
      }
    }
  /// <summary>
  /// get tick info
  /// </summary>
  class MTTicksAnswer : MTBaseAnswerJson
    {
    public string TransId { get; set; }
    /// <summary>
    /// From json get class MT5ConTime
    /// </summary>
    public List<MTTick> GetFromJson()
      {
      try
        {
        JavaScriptSerializer serializer = new JavaScriptSerializer {MaxJsonLength = MAX_LENGHT_JSON};
        serializer.RegisterConverters(new List<JavaScriptConverter> { new MTTickConverter() });
        return serializer.Deserialize<List<MTTick>>(ConfigJson);
        }
      catch(Exception e)
        {
        MTLog.Write(MTLogType.Error,string.Format("parsing tick from json failed, {0}",e));
        }
      return null;
      }
    }
  /// <summary>
  /// class parsin from json to List MTTick
  /// </summary>
  class MTTickConverter : JavaScriptConverter
    {
    private static readonly Type[] m_SupportedTypes = new[] { typeof(MTTick) };
    public override IEnumerable<Type> SupportedTypes { get { return m_SupportedTypes; } }
    /// <summary>
    /// Deserialize from MT server json
    /// </summary>
    /// <param name="dictionary">data from json</param>
    /// <param name="type"></param>
    /// <param name="serializer"></param>
    public override object Deserialize(IDictionary<string,object> dictionary,Type type,JavaScriptSerializer serializer)
      {
      //---
      return ParseTick(dictionary);
      }
    /// <summary>
    /// Parsing
    /// </summary>
    /// <param name="dictionary">list of object for parsing</param>
    private static MTTick ParseTick(IDictionary<string,object> dictionary)
      {
       if(dictionary == null)
         return null;
       //---
       MTTick obj = new MTTick();
       //---
       if(dictionary.ContainsKey("Symbol"))
         obj.Symbol = MTDataHelper.GetString(dictionary["Symbol"]);
       //---
       if(dictionary.ContainsKey("Digits"))
         obj.Digits = MTDataHelper.GetUInt32(dictionary["Digits"]);
       //---
       if(dictionary.ContainsKey("Bid"))
         obj.Bid = MTDataHelper.GetDouble(dictionary["Bid"]);
       //---
       if(dictionary.ContainsKey("Ask"))
         obj.Ask = MTDataHelper.GetDouble(dictionary["Ask"]);
       //---
       if(dictionary.ContainsKey("Last"))
         obj.Last = MTDataHelper.GetDouble(dictionary["Last"]);
       //---
       if(dictionary.ContainsKey("Volume"))
         obj.Volume = MTDataHelper.GetUInt64(dictionary["Volume"]);
       //---
       if(dictionary.ContainsKey("VolumeReal"))
         obj.VolumeReal = MTDataHelper.GetDouble(dictionary["VolumeReal"]);
       else
         obj.VolumeReal = (double)obj.Volume;
       //---
       if(dictionary.ContainsKey("Datetime"))
         obj.Datetime = MTDataHelper.GetUInt64(dictionary["Datetime"]);
       //---
       if(dictionary.ContainsKey("DatetimeMsc"))
         obj.DatetimeMsc = MTDataHelper.GetUInt64(dictionary["DatetimeMsc"]);
       else
         obj.DatetimeMsc = obj.Datetime*1000;
       //---
       return obj;
      }
    /// <summary>
    /// Serialize no used
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="serializer"></param>
    public override IDictionary<string,object> Serialize(object obj,JavaScriptSerializer serializer)
      {
      return null;
      }
    }
  /// <summary>
  /// get tick info
  /// </summary>
  class MTTickStatAnswer : MTBaseAnswerJson
    {
    public string TransId { get; set; }
    /// <summary>
    /// From json get class MT5ConTime
    /// </summary>
    public List<MTTickStat> GetFromJson()
      {
      try
        {
        JavaScriptSerializer serializer = new JavaScriptSerializer {MaxJsonLength = MAX_LENGHT_JSON};
        serializer.RegisterConverters(new List<JavaScriptConverter> { new MT5TickStatConverter() });
        return serializer.Deserialize<List<MTTickStat>>(ConfigJson);
        }
      catch(Exception e)
        {
        MTLog.Write(MTLogType.Error,string.Format("parsing tick stat from json failed, {0}",e));
        }
      return null;
      }
    }
  /// <summary>
  /// class parsin from json to List MTTickStat
  /// </summary>
  class MT5TickStatConverter : JavaScriptConverter
    {
    private static readonly Type[] m_SupportedTypes = new[] { typeof(MTTickStat) };
    public override IEnumerable<Type> SupportedTypes { get { return m_SupportedTypes; } }
    /// <summary>
    /// Deserialize from MT server json
    /// </summary>
    /// <param name="dictionary">data from json</param>
    /// <param name="type"></param>
    /// <param name="serializer"></param>
    public override object Deserialize(IDictionary<string,object> dictionary,Type type,JavaScriptSerializer serializer)
      {
      //---
      return ParseTickStat(dictionary);
      }
    /// <summary>
    /// Parsing
    /// </summary>
    /// <param name="dictionary">list of object for parsing</param>
    private static MTTickStat ParseTickStat(IDictionary<string,object> dictionary)
      {
      if(dictionary == null) return null;
      //---
      MTTickStat obj = new MTTickStat();
      //---
      if(dictionary.ContainsKey("Symbol"))
        obj.Symbol = MTDataHelper.GetString(dictionary["Symbol"]);
      //---
      if(dictionary.ContainsKey("Digits"))
        obj.Digits = MTDataHelper.GetUInt32(dictionary["Digits"]);
      //---
      if(dictionary.ContainsKey("Bid"))
        obj.Bid = MTDataHelper.GetDouble(dictionary["Bid"]);
      //---
      if(dictionary.ContainsKey("BidLow"))
        obj.BidLow = MTDataHelper.GetDouble(dictionary["BidLow"]);
      //---
      if(dictionary.ContainsKey("BidHigh"))
        obj.BidHigh = MTDataHelper.GetDouble(dictionary["BidHigh"]);
      //---
      if(dictionary.ContainsKey("BidDir"))
        obj.BidDir = (MTTickStat.EnDirection)MTDataHelper.GetUInt32(dictionary["BidDir"]);
      //---
      if(dictionary.ContainsKey("Ask"))
        obj.Ask = MTDataHelper.GetDouble(dictionary["Ask"]);
      //---
      if(dictionary.ContainsKey("AskLow"))
        obj.AskLow = MTDataHelper.GetDouble(dictionary["AskLow"]);
      //---
      if(dictionary.ContainsKey("AskHigh"))
        obj.AskHigh = MTDataHelper.GetDouble(dictionary["AskHigh"]);
      //---
      if(dictionary.ContainsKey("AskDir"))
        obj.AskDir = (MTTickStat.EnDirection)MTDataHelper.GetUInt32(dictionary["AskDir"]);
      //---
      if(dictionary.ContainsKey("Last"))
        obj.Last = MTDataHelper.GetDouble(dictionary["Last"]);
      //---
      if(dictionary.ContainsKey("LastLow"))
        obj.LastLow = MTDataHelper.GetDouble(dictionary["LastLow"]);
      //---
      if(dictionary.ContainsKey("LastHigh"))
        obj.LastHigh = MTDataHelper.GetDouble(dictionary["LastHigh"]);
      //---
      if(dictionary.ContainsKey("LastDir"))
        obj.LastDir = (MTTickStat.EnDirection)MTDataHelper.GetUInt32(dictionary["LastDir"]);
      //---
      if(dictionary.ContainsKey("Volume"))
        obj.Volume = MTDataHelper.GetUInt64(dictionary["Volume"]);
      if(dictionary.ContainsKey("VolumeReal"))
        obj.VolumeReal = MTDataHelper.GetDouble(dictionary["VolumeReal"]);
      else
        obj.VolumeReal = (double)obj.Volume;
      //---
      if(dictionary.ContainsKey("VolumeLow"))
        obj.VolumeLow = MTDataHelper.GetUInt64(dictionary["VolumeLow"]);
      if(dictionary.ContainsKey("VolumeLowReal"))
        obj.VolumeLowReal = MTDataHelper.GetDouble(dictionary["VolumeLowReal"]);
      else
        obj.VolumeLowReal = (double)obj.VolumeLow;
      //---
      if(dictionary.ContainsKey("VolumeHigh"))
        obj.VolumeHigh = MTDataHelper.GetUInt64(dictionary["VolumeHigh"]);
      if(dictionary.ContainsKey("VolumeHighReal"))
        obj.VolumeHighReal = MTDataHelper.GetDouble(dictionary["VolumeHighReal"]);
      else
        obj.VolumeHighReal = (double)obj.VolumeHigh;
      //---
      if(dictionary.ContainsKey("VolumeDir"))
        obj.VolumeDir = (MTTickStat.EnDirection)MTDataHelper.GetUInt32(dictionary["VolumeDir"]);
      //---
      if(dictionary.ContainsKey("TradeDeals"))
        obj.TradeDeals = MTDataHelper.GetUInt64(dictionary["TradeDeals"]);
      //---
      if(dictionary.ContainsKey("TradeVolume"))
        obj.TradeVolume = MTDataHelper.GetUInt64(dictionary["TradeVolume"]);
      if(dictionary.ContainsKey("TradeVolumeReal"))
        obj.TradeVolumeReal = MTDataHelper.GetDouble(dictionary["TradeVolumeReal"]);
      else
        obj.TradeVolumeReal = (double)obj.TradeVolume;
      //---
      if(dictionary.ContainsKey("TradeTurnover"))
        obj.TradeTurnover = MTDataHelper.GetUInt64(dictionary["TradeTurnover"]);
      //---
      if(dictionary.ContainsKey("TradeInterest"))
        obj.TradeInterest = MTDataHelper.GetUInt64(dictionary["TradeInterest"]);
      //---
      if(dictionary.ContainsKey("TradeBuyOrders"))
        obj.TradeBuyOrders = MTDataHelper.GetUInt64(dictionary["TradeBuyOrders"]);
      //---
      if(dictionary.ContainsKey("TradeBuyVolume"))
        obj.TradeBuyVolume = MTDataHelper.GetUInt64(dictionary["TradeBuyVolume"]);
      if(dictionary.ContainsKey("TradeBuyVolumeReal"))
        obj.TradeBuyVolumeReal = MTDataHelper.GetDouble(dictionary["TradeBuyVolumeReal"]);
      else
        obj.TradeBuyVolumeReal = (double)obj.TradeBuyVolume;
      //---
      if(dictionary.ContainsKey("TradeSellOrders"))
        obj.TradeSellOrders = MTDataHelper.GetUInt64(dictionary["TradeSellOrders"]);
      //---
      if(dictionary.ContainsKey("TradeSellVolume"))
        obj.TradeSellVolume = MTDataHelper.GetUInt64(dictionary["TradeSellVolume"]);
      if(dictionary.ContainsKey("TradeSellVolumeReal"))
        obj.TradeSellVolumeReal = MTDataHelper.GetDouble(dictionary["TradeSellVolumeReal"]);
      else
        obj.TradeSellVolumeReal = (double)obj.TradeSellVolume;
      //---
      if(dictionary.ContainsKey("PriceOpen"))
        obj.PriceOpen = MTDataHelper.GetDouble(dictionary["PriceOpen"]);
      //---
      if(dictionary.ContainsKey("PriceClose"))
        obj.PriceClose = MTDataHelper.GetDouble(dictionary["PriceClose"]);
      //---
      if(dictionary.ContainsKey("PriceChange"))
        obj.PriceChange = MTDataHelper.GetDouble(dictionary["PriceChange"]);
      //---
      if(dictionary.ContainsKey("PriceVolatility"))
        obj.PriceVolatility = MTDataHelper.GetDouble(dictionary["PriceVolatility"]);
      //---
      if(dictionary.ContainsKey("PriceTheoretical"))
        obj.PriceTheoretical = MTDataHelper.GetDouble(dictionary["PriceTheoretical"]);
      //---
      return obj;
      }
    /// <summary>
    /// Serialize no used
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="serializer"></param>
    public override IDictionary<string,object> Serialize(object obj,JavaScriptSerializer serializer)
      {
      return null;
      }
    }
  }
