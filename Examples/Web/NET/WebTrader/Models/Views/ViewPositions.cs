﻿//+------------------------------------------------------------------+
//|                                             MetaTrader 5 Web API |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
using System.Collections.Generic;
using MetaQuotes.MT5WebAPI.Common;
//---
namespace WebTrader.Models.Views
  {
  public class ViewPositions
    {
    public List<MTPosition> Positions { get; set; }
    public uint CurrentPage { get; set; }
    }
  }