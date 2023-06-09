﻿//+------------------------------------------------------------------+
//|                                             MetaTrader 5 Web API |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Web.Script.Serialization;
using MetaQuotes.MT5WebAPI.Common.Utils;
//---
namespace MetaQuotes.MT5WebAPI.Common.Protocol
  {
  /// <summary>
  /// work with group
  /// </summary>
  class MTGroupBase : MTAPIBase
    {
    public MTGroupBase(MTAsyncConnect connect) : base(connect) { }
    /// <summary>
    ///  Get total group
    /// </summary>
    /// <param name="total">total groups</param>
    public MTRetCode GroupTotal(out int total)
      {
      total = 0;
      //--- get answer
      byte[] answer;
      //--- send request
      if((answer = Send(MTProtocolConsts.WEB_CMD_GROUP_TOTAL,null)) == null)
        {
        MTLog.Write(MTLogType.Error,"send group total failed");
        return MTRetCode.MT_RET_ERR_NETWORK;
        }
      //---
      string answerStr = MTUtils.GetString(answer);
      if(MTLog.IsWriteDebugLog) MTLog.Write(MTLogType.Debug,string.Format("result answer: {0}",answerStr));
      //---
      MTRetCode errorCode;
      //--- parse answer
      if((errorCode = ParseGroupTotal(answerStr,out total)) != MTRetCode.MT_RET_OK)
        {
        MTLog.Write(MTLogType.Error,string.Format("parse group total failed: {0}",MTFormat.GetErrorStandart(errorCode)));
        return errorCode;
        }
      //---
      return MTRetCode.MT_RET_OK;
      }
    /// <summary>
    /// Get group config
    /// </summary>
    /// <param name="pos">from 0 to total group</param>
    /// <param name="conGroup">group config</param>
    public MTRetCode GroupNext(uint pos,out MTConGroup conGroup)
      {
      conGroup = null;
      //--- send request
      Dictionary<string,string> data = new Dictionary<string,string>();
      data.Add(MTProtocolConsts.WEB_PARAM_INDEX,pos.ToString());
      //--- get answer
      byte[] answer;
      //--- send request
      if((answer = Send(MTProtocolConsts.WEB_CMD_GROUP_NEXT,data)) == null)
        {
        MTLog.Write(MTLogType.Error,"send group next failed");
        return MTRetCode.MT_RET_ERR_NETWORK;
        }
      //---
      string answerStr = MTUtils.GetString(answer);
      if(MTLog.IsWriteDebugLog) MTLog.Write(MTLogType.Debug,string.Format("result answer: {0}",answerStr));
      //--- parse answer
      MTRetCode errorCode;
      if((errorCode = ParseGroup(MTProtocolConsts.WEB_CMD_GROUP_NEXT,answerStr,out conGroup)) != MTRetCode.MT_RET_OK)
        {
        MTLog.Write(MTLogType.Error,string.Format("parse group next failed: {0}",MTFormat.GetErrorStandart(errorCode)));
        return errorCode;
        }
      //---
      return MTRetCode.MT_RET_OK;
      }
    /// <summary>
    /// Get group config
    /// </summary>
    /// <param name="name">name group</param>
    /// <param name="conGroup">group config</param>
    public MTRetCode GroupGet(string name,out MTConGroup conGroup)
      {
      conGroup = null;
      //--- send request
      Dictionary<string,string> data = new Dictionary<string,string>();
      data.Add(MTProtocolConsts.WEB_PARAM_GROUP,name);
      //--- get answer
      byte[] answer;
      //--- send request
      if((answer = Send(MTProtocolConsts.WEB_CMD_GROUP_GET,data)) == null)
        {
        MTLog.Write(MTLogType.Error,"send group get failed");
        return MTRetCode.MT_RET_ERR_NETWORK;
        }
      //---
      string answerStr = MTUtils.GetString(answer);
      if(MTLog.IsWriteDebugLog) MTLog.Write(MTLogType.Debug,string.Format("result answer: {0}",answerStr));
      //--- parse answer
      MTRetCode errorCode;
      if((errorCode = ParseGroup(MTProtocolConsts.WEB_CMD_GROUP_GET,answerStr,out conGroup)) != MTRetCode.MT_RET_OK)
        {
        MTLog.Write(MTLogType.Error,string.Format("parse group get failed: {0}",MTFormat.GetErrorStandart(errorCode)));
        return errorCode;
        }
      //---
      return MTRetCode.MT_RET_OK;
      }
    /// <summary>
    /// Update group config
    /// </summary>
    /// <param name="group">name group</param>
    /// <param name="newGroup">new group config</param>
    public MTRetCode GroupAdd(MTConGroup group,out MTConGroup newGroup)
      {
      newGroup = null;
      //--- send request
      Dictionary<string,string> data = new Dictionary<string,string>();
      data.Add(MTProtocolConsts.WEB_PARAM_BODYTEXT,MTGroupJson.ToJson(group));
      //--- get answer
      byte[] answer;
      //--- send request
      if((answer = Send(MTProtocolConsts.WEB_CMD_GROUP_ADD,data)) == null)
        {
        MTLog.Write(MTLogType.Error,"send group add failed");
        return MTRetCode.MT_RET_ERR_NETWORK;
        }
      //---
      string answerStr = MTUtils.GetString(answer);
      if(MTLog.IsWriteDebugLog) MTLog.Write(MTLogType.Debug,string.Format("result answer: {0}",answerStr));
      //--- parse answer
      MTRetCode errorCode;
      if((errorCode = ParseGroup(MTProtocolConsts.WEB_CMD_GROUP_ADD,answerStr,out newGroup)) != MTRetCode.MT_RET_OK)
        {
        MTLog.Write(MTLogType.Error,string.Format("parse group add failed: {0}",MTFormat.GetErrorStandart(errorCode)));
        return errorCode;
        }
      //---
      return MTRetCode.MT_RET_OK;
      }
    /// <summary>
    /// Delete group
    /// </summary>
    /// <param name="name">name group</param>
    /// <returns></returns>
    public MTRetCode GroupDelete(string name)
      {
      //--- send request
      Dictionary<string,string> data = new Dictionary<string,string>();
      data.Add(MTProtocolConsts.WEB_PARAM_GROUP,name);
      //--- get answer
      byte[] answer;
      //---
      if((answer = Send(MTProtocolConsts.WEB_CMD_GROUP_DELETE,data)) == null)
        {
        MTLog.Write(MTLogType.Error,"send group delete failed");
        return MTRetCode.MT_RET_ERR_NETWORK;
        }
      //---
      string answerStr = MTUtils.GetString(answer);
      if(MTLog.IsWriteDebugLog) MTLog.Write(MTLogType.Debug,string.Format("result answer: {0}",answerStr));
      //---
      MTRetCode errorCode;
      //--- parse answer
      if((errorCode = ParseEmptyResult(MTProtocolConsts.WEB_CMD_GROUP_DELETE,answerStr)) != MTRetCode.MT_RET_OK)
        {
        MTLog.Write(MTLogType.Error,string.Format("parse group delete failed: {0}",MTFormat.GetErrorStandart(errorCode)));
        return errorCode;
        }
      //---
      return MTRetCode.MT_RET_OK;
      }
    /// <summary>
    /// Check answer from MetaTrader 5 server
    /// </summary>
    /// <param name="answer">answer from MT5 sever</param>
    /// <param name="total">total groups</param>
    private static MTRetCode ParseGroupTotal(string answer,out int total)
      {
      int pos = 0;
      total = 0;
      //--- get command answer
      string command = MTParseProtocol.GetCommand(answer,ref pos);
      if(command != MTProtocolConsts.WEB_CMD_GROUP_TOTAL)
        {
        MTLog.Write(MTLogType.Error,string.Format("answer command '{0}' is incorrect, wait {1}",command,MTProtocolConsts.WEB_CMD_GROUP_TOTAL));
        return MTRetCode.MT_RET_ERR_DATA;
        }
      //---
      MTGroupTotalAnswer groupAnswer = new MTGroupTotalAnswer();
      //--- get param
      int posEnd = -1;
      MTAnswerParam param;
      while((param = MTParseProtocol.GetNextParam(answer,ref pos,ref posEnd)) != null)
        {
        switch(param.Name)
          {
          case MTProtocolConsts.WEB_PARAM_RETCODE:
          groupAnswer.RetCode = param.Value;
          break;
          case MTProtocolConsts.WEB_PARAM_TOTAL:
          groupAnswer.Total = param.Value;
          break;
          }
        }
      //---
      MTRetCode errorCode;
      //--- check ret code
      if((errorCode = MTParseProtocol.GetRetCode(groupAnswer.RetCode)) != MTRetCode.MT_RET_OK) return errorCode;
      //---
      if(!int.TryParse(groupAnswer.Total,out total)) return MTRetCode.MT_RET_REPORT_NODATA;
      return MTRetCode.MT_RET_OK;
      }
    /// <summary>
    /// check answer from MetaTrader 5 server
    /// </summary>
    /// <param name="command">command send</param>
    /// <param name="answer">answer from MT5 sever</param>
    /// <param name="conGroup">result pasing</param>
    private static MTRetCode ParseGroup(string command,string answer,out MTConGroup conGroup)
      {
      conGroup = null;
      int pos = 0;
      //--- get command answer
      string commandReal = MTParseProtocol.GetCommand(answer,ref pos);
      if(command != commandReal)
        {
        MTLog.Write(MTLogType.Error,string.Format("answer command '{0}' is incorrect, wait {1}",command,commandReal));
        return MTRetCode.MT_RET_ERR_DATA;
        }
      //---
      MTGroupAnswer groupAnswer = new MTGroupAnswer();
      //--- get param
      int posEnd = -1;
      MTAnswerParam param;
      while((param = MTParseProtocol.GetNextParam(answer,ref pos,ref posEnd)) != null)
        {
        switch(param.Name)
          {
          case MTProtocolConsts.WEB_PARAM_RETCODE:
          groupAnswer.RetCode = param.Value;
          break;
          }
        }
      //---
      MTRetCode errorCode;
      //--- check ret code
      if((errorCode = MTParseProtocol.GetRetCode(groupAnswer.RetCode)) != MTRetCode.MT_RET_OK) return errorCode;
      //--- get json
      if((groupAnswer.ConfigJson = MTParseProtocol.GetJson(answer,posEnd)) == null) return MTRetCode.MT_RET_REPORT_NODATA;
      //--- parsing Json
      conGroup = groupAnswer.GetFromJson();
      //--- parsing empty
      if(conGroup == null) return MTRetCode.MT_RET_REPORT_NODATA;
      return MTRetCode.MT_RET_OK;
      }
    }
  /// <summary>
  /// Answer on request group_total
  /// </summary>
  internal class MTGroupTotalAnswer : MTBaseAnswer
    {
    public string Total { get; set; }
    }
  /// <summary>
  /// get group info
  /// </summary>
  internal class MTGroupAnswer : MTBaseAnswerJson
    {
    /// <summary>
    /// From json get class MT5ConTime
    /// </summary>
    public MTConGroup GetFromJson()
      {
      try
        {
        JavaScriptSerializer serializer = new JavaScriptSerializer { MaxJsonLength = MAX_LENGHT_JSON };
        serializer.RegisterConverters(new List<JavaScriptConverter> { new MTConGroupConverter() });
        return serializer.Deserialize<MTConGroup>(ConfigJson);
        }
      catch(Exception e)
        {
        MTLog.Write(MTLogType.Error,string.Format("parsing common config from json failed, {0}",e));
        }
      return null;
      }
    }
  /// <summary>
  /// class parsin from json to MTConGroup
  /// </summary>
  internal class MTConGroupConverter : JavaScriptConverter
    {
    private static readonly Type[] m_SupportedTypes = new[] { typeof(MTConGroup) };
    public override IEnumerable<Type> SupportedTypes { get { return m_SupportedTypes; } }
    /// <summary>
    /// Deserialize from MT server json
    /// </summary>
    /// <param name="dictionary">data from json</param>
    /// <param name="type"></param>
    /// <param name="serializer"></param>

    public override object Deserialize(IDictionary<string,object> dictionary,Type type,JavaScriptSerializer serializer)
      {
      if(dictionary == null) return null;
      //---
      MTConGroup obj = new MTConGroup();
      //---
      if(dictionary.ContainsKey("Group"))
        obj.Group = MTDataHelper.GetString(dictionary["Group"]);
      //--- 
      if(dictionary.ContainsKey("Server"))
        obj.Server = MTDataHelper.GetUInt64(dictionary["Server"]);
      //---
      if(dictionary.ContainsKey("PermissionsFlags"))
        obj.PermissionsFlags = (MTConGroup.EnPermissionsFlags)MTDataHelper.GetUInt64(dictionary["PermissionsFlags"]);
      //---
      if(dictionary.ContainsKey("AuthMode"))
        obj.AuthMode = (MTConGroup.EnAuthMode)MTDataHelper.GetUInt32(dictionary["AuthMode"]);
      //---
      if(dictionary.ContainsKey("AuthPasswordMin"))
        obj.AuthPasswordMin = MTDataHelper.GetUInt32(dictionary["AuthPasswordMin"]);
      //---
      if(dictionary.ContainsKey("AuthOTPMode"))
        obj.AuthOTPMode = (MTConGroup.EnAuthOTPMode)MTDataHelper.GetUInt32(dictionary["AuthOTPMode"]);
      //---
      if(dictionary.ContainsKey("Company"))
        obj.Company = MTDataHelper.GetString(dictionary["Company"]);
      //---
      if(dictionary.ContainsKey("CompanyPage"))
        obj.CompanyPage = MTDataHelper.GetString(dictionary["CompanyPage"]);
      //---
      if(dictionary.ContainsKey("CompanyEmail"))
        obj.CompanyEmail = MTDataHelper.GetString(dictionary["CompanyEmail"]);
      //---
      if(dictionary.ContainsKey("CompanySupportPage"))
        obj.CompanySupportPage = MTDataHelper.GetString(dictionary["CompanySupportPage"]);
      //---
      if(dictionary.ContainsKey("CompanySupportEmail"))
        obj.CompanySupportEmail = MTDataHelper.GetString(dictionary["CompanySupportEmail"]);
      //---
      if(dictionary.ContainsKey("CompanyCatalog"))
        obj.CompanyCatalog = MTDataHelper.GetString(dictionary["CompanyCatalog"]);
      //---
      if(dictionary.ContainsKey("Currency"))
        obj.Currency = MTDataHelper.GetString(dictionary["Currency"]);
      //---
      if(dictionary.ContainsKey("CurrencyDigits"))
        obj.CurrencyDigits = MTDataHelper.GetUInt32(dictionary["CurrencyDigits"]);
      //---
      if(dictionary.ContainsKey("ReportsMode"))
        obj.ReportsMode = (MTConGroup.EnReportsMode)MTDataHelper.GetUInt32(dictionary["ReportsMode"]);
      //---
      if(dictionary.ContainsKey("ReportsFlags"))
        obj.ReportsFlags = (MTConGroup.EnReportsFlags)MTDataHelper.GetUInt64(dictionary["ReportsFlags"]);
      //---
      if(dictionary.ContainsKey("ReportsSMTP"))
        obj.ReportsSMTP = MTDataHelper.GetString(dictionary["ReportsSMTP"]);
      //---
      if(dictionary.ContainsKey("ReportsSMTPLogin"))
        obj.ReportsSMTPLogin = MTDataHelper.GetString(dictionary["ReportsSMTPLogin"]);
      //---
      if(dictionary.ContainsKey("ReportsSMTPPass"))
        obj.ReportsSMTPPass = MTDataHelper.GetString(dictionary["ReportsSMTPPass"]);
      //---
      if(dictionary.ContainsKey("NewsMode"))
        obj.NewsMode = (MTConGroup.EnNewsMode)MTDataHelper.GetUInt32(dictionary["NewsMode"]);
      //---
      if(dictionary.ContainsKey("NewsCategory"))
        obj.NewsCategory = MTDataHelper.GetString(dictionary["NewsCategory"]);
      //---
      if(dictionary.ContainsKey("NewsLangs"))
        obj.NewsLangs = ParsingNewsLang(dictionary["NewsLangs"] as ArrayList);
      //---
      if(dictionary.ContainsKey("MailMode"))
        obj.MailMode = (MTConGroup.EnMailMode)MTDataHelper.GetUInt32(dictionary["MailMode"]);
      //---
      if(dictionary.ContainsKey("TradeFlags"))
        obj.TradeFlags = (EnTradeFlags)MTDataHelper.GetUInt64(dictionary["TradeFlags"]);
      //---
      if(dictionary.ContainsKey("TradeTransferMode"))
        obj.TradeTransferMode = (MTConGroup.EnTransferMode)MTDataHelper.GetUInt32(dictionary["TradeTransferMode"]);
      //---
      if(dictionary.ContainsKey("TradeInterestrate"))
        obj.TradeInterestrate = MTDataHelper.GetDouble(dictionary["TradeInterestrate"]);
      //---
      if(dictionary.ContainsKey("TradeVirtualCredit"))
        obj.TradeVirtualCredit = MTDataHelper.GetDouble(dictionary["TradeVirtualCredit"]);
      //---
      if(dictionary.ContainsKey("MarginMode"))
        obj.MarginMode = (MTConGroup.EnMarginMode)MTDataHelper.GetUInt32(dictionary["MarginMode"]);
      //---
      if(dictionary.ContainsKey("MarginSOMode"))
        obj.MarginSOMode = (MTConGroup.EnStopOutMode)MTDataHelper.GetUInt32(dictionary["MarginSOMode"]);
      //---
      if(dictionary.ContainsKey("MarginFreeMode"))
        obj.MarginFreeMode = (MTConGroup.EnFreeMarginMode)MTDataHelper.GetUInt32(dictionary["MarginFreeMode"]);
      //---
      if(dictionary.ContainsKey("MarginCall"))
        obj.MarginCall = MTDataHelper.GetDouble(dictionary["MarginCall"]);
      //---
      if(dictionary.ContainsKey("MarginStopOut"))
        obj.MarginStopOut = MTDataHelper.GetDouble(dictionary["MarginStopOut"]);
      //---
      if(dictionary.ContainsKey("MarginFreeProfitMode"))
        obj.MarginFreeProfitMode = (MTConGroup.EnMarginFreeProfitMode)MTDataHelper.GetUInt32(dictionary["MarginFreeProfitMode"]);
      //---
      if(dictionary.ContainsKey("DemoLeverage"))
        obj.DemoLeverage = MTDataHelper.GetUInt32(dictionary["DemoLeverage"]);
      //---
      if(dictionary.ContainsKey("DemoDeposit"))
        obj.DemoDeposit = MTDataHelper.GetDouble(dictionary["DemoDeposit"]);
      //---
      if(dictionary.ContainsKey("LimitHistory"))
        obj.LimitHistory = (MTConGroup.EnHistoryLimit)MTDataHelper.GetUInt32(dictionary["LimitHistory"]);
      //---
      if(dictionary.ContainsKey("LimitOrders"))
        obj.LimitOrders = MTDataHelper.GetUInt32(dictionary["LimitOrders"]);
      //---
      if(dictionary.ContainsKey("LimitSymbols"))
        obj.LimitSymbols = MTDataHelper.GetUInt32(dictionary["LimitSymbols"]);
      //---
      if (dictionary.ContainsKey("LimitPositions"))
        obj.LimitPositions = MTDataHelper.GetUInt32(dictionary["LimitPositions"]);
      //---
      if (dictionary.ContainsKey("Commissions"))
        obj.Commissions = ParsingCommissions(dictionary["Commissions"] as ArrayList);
      //---
      if(dictionary.ContainsKey("Symbols"))
        obj.Symbols = ParsingSymbols(dictionary["Symbols"] as ArrayList);
      //---
      return obj;
      }
    /// <summary>
    /// paring list of symbols
    /// </summary>
    /// <param name="symbols"></param>

    private static List<MTConGroupSymbol> ParsingSymbols(ArrayList symbols)
      {
      if(symbols == null) return null;
      //---
      List<MTConGroupSymbol> result = new List<MTConGroupSymbol>();
      //---
      foreach(Dictionary<string,object> symbol in symbols)
        {
        try
          {
          result.Add(MTConSymbolConverter.ParseGroupSymbol(symbol));
          }
        catch(Exception e)
          {
          MTLog.Write(MTLogType.Error,string.Format("parsing symbols failed, {0}",e));
          }
        }
      //---
      return result;
      }
    /// <summary>
    /// parsing news languages
    /// </summary>
    /// <param name="languages">list languages</param>

    private static List<uint> ParsingNewsLang(ArrayList languages)
      {
      if(languages == null) return null;
      //---
      List<uint> result = new List<uint>();
      foreach(string lang in languages)
        {
        result.Add(MTDataHelper.GetUInt32(lang));
        }
      return result;
      }
    /// <summary>
    /// parsing list commissions
    /// </summary>
    /// <param name="commissions">list commssions need parsing</param>
    private static List<MTConCommission> ParsingCommissions(ArrayList commissions)
      {
      if(commissions == null) return null;
      //---
      List<MTConCommission> result = new List<MTConCommission>();
      //---
      foreach(Dictionary<string,object> commissionInfo in commissions)
        {
        MTConCommission temp = ParsingComission(commissionInfo);
        if(temp != null) result.Add(temp);
        }
      //---     
      return result;
      }
    /// <summary>
    /// parsing MTCommission
    /// </summary>
    /// <param name="dictionary">data from json</param>
    /// <returns></returns>
    private static MTConCommission ParsingComission(Dictionary<string,object> dictionary)
      {
      if(dictionary == null) return null;
      //---
      MTConCommission obj = new MTConCommission();
      //---
      if(dictionary.ContainsKey("Name"))
        obj.Name = MTDataHelper.GetString(dictionary["Name"]);
      //---
      if(dictionary.ContainsKey("Description"))
        obj.Description = MTDataHelper.GetString(dictionary["Description"]);
      //---
      if(dictionary.ContainsKey("Path"))
        obj.Path = MTDataHelper.GetString(dictionary["Path"]);
      //---
      if(dictionary.ContainsKey("Mode"))
        obj.Mode = (MTConCommission.EnCommMode)MTDataHelper.GetUInt32(dictionary["Mode"]);
      //---
      if(dictionary.ContainsKey("RangeMode"))
        obj.RangeMode = (MTConCommission.EnCommRangeMode)MTDataHelper.GetUInt32(dictionary["RangeMode"]);
      //---
      if(dictionary.ContainsKey("ChargeMode"))
        obj.ChargeMode = (MTConCommission.EnCommChargeMode)MTDataHelper.GetUInt32(dictionary["ChargeMode"]);
      //---
      if (dictionary.ContainsKey("TurnoverCurrency"))
        obj.TurnoverCurrency = MTDataHelper.GetString(dictionary["TurnoverCurrency"]);
      //---
      if (dictionary.ContainsKey("EntryMode"))
        obj.EntryMode = (MTConCommission.EnCommEntryMode)MTDataHelper.GetUInt32(dictionary["EntryMode"]);
      //---
      if (dictionary.ContainsKey("Tiers"))
        obj.Tiers = ParsingTiers(dictionary["Tiers"] as ArrayList);
      //---
      return obj;
      }
    /// <summary>
    /// parsing list commission tiers
    /// </summary>
    /// <param name="tiers">list commision tiers need parsing</param>
    private static List<MTConCommTier> ParsingTiers(ArrayList tiers)
      {
      if(tiers == null) return null;
      //---
      List<MTConCommTier> result = new List<MTConCommTier>();
      //---
      foreach(Dictionary<string,object> tierInfo in tiers)
        {
        MTConCommTier temp = ParsingTier(tierInfo);
        if(temp != null) result.Add(temp);
        }
      //---
      return result;
      }
    /// <summary>
    /// parsing MTConCommTier
    /// </summary>
    /// <param name="dictionary">data from json</param>
    /// <returns></returns>
    private static MTConCommTier ParsingTier(Dictionary<string,object> dictionary)
      {
      if(dictionary == null) return null;
      //---
      MTConCommTier obj = new MTConCommTier();
      //---
      if(dictionary.ContainsKey("Mode"))
        obj.Mode = (MTConCommTier.EnCommissionMode)MTDataHelper.GetUInt32(dictionary["Mode"]);
      //---
      if(dictionary.ContainsKey("Type"))
        obj.Type = (MTConCommTier.EnCommissionVolumeType)MTDataHelper.GetUInt32(dictionary["Type"]);
      //---
      if(dictionary.ContainsKey("Value"))
        obj.Value = MTDataHelper.GetDouble(dictionary["Value"]);
      //---
      if(dictionary.ContainsKey("Minimal"))
        obj.Minimal = MTDataHelper.GetDouble(dictionary["Minimal"]);
      //---
      if(dictionary.ContainsKey("RangeFrom"))
        obj.RangeFrom = MTDataHelper.GetDouble(dictionary["RangeFrom"]);
      //---
      if(dictionary.ContainsKey("RangeTo"))
        obj.RangeTo = MTDataHelper.GetDouble(dictionary["RangeTo"]);
      //---
      if (dictionary.ContainsKey("Currency"))
        obj.Currency = MTDataHelper.GetString(dictionary["Currency"]);
      //---
      if(dictionary.ContainsKey("Maximal"))
        obj.Maximal = MTDataHelper.GetDouble(dictionary["Maximal"]);
      //---
      return obj;
      }
    /// <summary>
    /// serialize not used
    /// </summary>
    /// <param name="obj"></param>
    /// <param name="serializer"></param>
    public override IDictionary<string,object> Serialize(object obj,JavaScriptSerializer serializer)
      {
      return null;
      }
    }
  /// <summary>
  /// class get json from object
  /// </summary>
  internal class MTGroupJson
    {
    /// <summary>
    /// to json
    /// </summary>
    /// <param name="group">group data</param>
    /// <returns></returns>
    public static string ToJson(MTConGroup group)
      {
      if(group == null) return "{}";
      //---
      JSONWriter writer = new JSONWriter();
      //---
      writer.WriteBeginObject();
      writer.WriteAttribute("Group",group.Group);
      writer.WriteAttribute("Server",group.Server);
      writer.WriteAttribute("PermissionsFlags",(ulong)group.PermissionsFlags);
      writer.WriteAttribute("AuthMode",(uint)group.AuthMode);
      writer.WriteAttribute("AuthPasswordMin",group.AuthPasswordMin);
      writer.WriteAttribute("AuthOTPMode",(uint)group.AuthOTPMode);
      writer.WriteAttribute("Company",group.Company);
      writer.WriteAttribute("CompanyPage",group.CompanyPage);
      writer.WriteAttribute("CompanyEmail",group.CompanyEmail);
      writer.WriteAttribute("CompanySupportPage",group.CompanySupportPage);
      writer.WriteAttribute("CompanySupportEmail",group.CompanySupportEmail);
      writer.WriteAttribute("CompanyCatalog",group.CompanyCatalog);
      writer.WriteAttribute("Currency",group.Currency);
      writer.WriteAttribute("CurrencyDigits",group.CurrencyDigits);
      writer.WriteAttribute("ReportsMode",(uint)group.ReportsMode);
      writer.WriteAttribute("ReportsFlags",(uint)group.ReportsFlags);
      writer.WriteAttribute("ReportsSMTP",group.ReportsSMTP);
      writer.WriteAttribute("ReportsSMTPLogin",group.ReportsSMTPLogin);
      writer.WriteAttribute("ReportsSMTPPass",group.ReportsSMTPPass);
      writer.WriteAttribute("NewsMode",(uint)group.NewsMode);
      writer.WriteAttribute("NewsCategory",group.NewsCategory);
      writer.WriteAttribute("NewsLangs",group.NewsLangs);
      writer.WriteAttribute("MailMode",(uint)group.MailMode);
      writer.WriteAttribute("TradeFlags",(ulong)group.TradeFlags);
      writer.WriteAttribute("TradeTransferMode",(uint)group.TradeTransferMode);
      writer.WriteAttribute("TradeInterestrate",group.TradeInterestrate);
      writer.WriteAttribute("TradeVirtualCredit",group.TradeVirtualCredit);
      writer.WriteAttribute("MarginMode",(uint)group.MarginMode);
      writer.WriteAttribute("MarginSOMode",(uint)group.MarginSOMode);
      writer.WriteAttribute("MarginFreeMode",(uint)group.MarginFreeMode);
      writer.WriteAttribute("MarginCall",group.MarginCall);
      writer.WriteAttribute("MarginStopOut",group.MarginStopOut);
      writer.WriteAttribute("MarginFreeProfitMode",(uint)group.MarginFreeProfitMode);
      writer.WriteAttribute("DemoLeverage",group.DemoLeverage);
      writer.WriteAttribute("DemoDeposit",group.DemoDeposit);
      writer.WriteAttribute("LimitHistory",(uint)group.LimitHistory);
      writer.WriteAttribute("LimitOrders",group.LimitOrders);
      writer.WriteAttribute("LimitSymbols",group.LimitSymbols);
      writer.WriteAttribute("LimitPositions", group.LimitPositions);
      writer.WriteAttribute("Commissions",GetCommissionsJson(group.Commissions),false);
      writer.WriteAttribute("Symbols",MTGroupSymbolsJson.ListToJson(group.Symbols),false);
      //---
      writer.WriteEndObject();
      //---
      return writer.ToString();
      }
    /// <summary>
    /// json list MTConCommision
    /// </summary>
    /// <param name="commissions">list of commisions</param>
    /// <returns></returns>
    private static string GetCommissionsJson(List<MTConCommission> commissions)
      {
      StringBuilder sb = new StringBuilder();
      //---
      foreach(MTConCommission commission in commissions)
        {
        sb.AppendFormat("{0}{1}",sb.Length == 0 ? "" : ",",GetCommisionJson(commission));
        }
      //---
      return "[" + sb + "]";
      }
    /// <summary>
    /// get json MTConCommision
    /// </summary>
    /// <param name="commission">MTConCommision</param>
    /// <returns></returns>
    private static string GetCommisionJson(MTConCommission commission)
      {
      if(commission == null) return "{}";
      //---
      JSONWriter writer = new JSONWriter();
      //---
      writer.WriteBeginObject();
      writer.WriteAttribute("Name",commission.Name);
      writer.WriteAttribute("Description",commission.Description);
      writer.WriteAttribute("Path",commission.Path);
      writer.WriteAttribute("Mode",(uint)commission.Mode);
      writer.WriteAttribute("RangeMode",(uint)commission.RangeMode);
      writer.WriteAttribute("ChargeMode",(uint)commission.ChargeMode);
      writer.WriteAttribute("TurnoverCurrency", commission.TurnoverCurrency);
      writer.WriteAttribute("EntryMode", (uint)commission.EntryMode);
      writer.WriteAttribute("Tiers",GetTiersJson(commission.Tiers),false);
      //---
      writer.WriteEndObject();
      //---
      return writer.ToString();
      }
    /// <summary>
    /// get json list MTConCommTier
    /// </summary>
    /// <param name="tiers"></param>
    /// <returns>json</returns>
    private static string GetTiersJson(List<MTConCommTier> tiers)
      {
      StringBuilder sb = new StringBuilder();
      //---
      foreach(MTConCommTier tier in tiers)
        {
        sb.AppendFormat("{0}{1}",sb.Length == 0 ? "" : ",",GetTierJson(tier));
        }
      //---
      return "[" + sb + "]";
      }
    /// <summary>
    /// get json MTConCommTier
    /// </summary>
    /// <param name="tier">MTConCommTier</param>
    /// <returns></returns>
    private static string GetTierJson(MTConCommTier tier)
      {
      if(tier == null) return "{}";
      //---
      JSONWriter writer = new JSONWriter();
      //---
      writer.WriteBeginObject();
      writer.WriteAttribute("Mode",(uint)tier.Mode);
      writer.WriteAttribute("Type",(uint)tier.Type);
      writer.WriteAttribute("Value",tier.Value);
      writer.WriteAttribute("Minimal",tier.Minimal);
      writer.WriteAttribute("Maximal",tier.Maximal);
      writer.WriteAttribute("RangeFrom",tier.RangeFrom);
      writer.WriteAttribute("RangeTo",tier.RangeTo);
      writer.WriteAttribute("Currency", tier.Currency);
      //---
      writer.WriteEndObject();
      //---
      return writer.ToString();
      }
    }
  }
