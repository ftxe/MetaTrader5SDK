﻿//+------------------------------------------------------------------+
//|                                             MetaTrader 5 Web API |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
using System;
using System.Collections.Generic;
using System.Web.Mvc;
using MetaQuotes.MT5WebAPI;
using MetaQuotes.MT5WebAPI.Common;
using WebTrader.Models;
using WebTrader.Models.Core.Controllers;
using WebTrader.Models.Users;
using WebTrader.Models.Views;
//---
namespace WebTrader.Controllers
  {
  /// <summary>
  /// View pages of history data
  /// </summary>
  public class HistoryController : BaseAsyncController
    {
    public const int ORDERS_PAGE = 20;
    private List<MTOrder> m_Orders;
    private uint m_Page;
    /// <summary>
    /// Index async view page
    /// </summary>
    /// <param name="page">number of page</param>
    public void IndexAsync(uint? page)
      {
      if(!UserState.IsLoggedIn)
        {
        Http404NotFound("");
        return;
        }
      //---
      AsyncManager.OutstandingOperations.Increment();
      //---
      m_Page = page ?? 1;
      //---
      QueueData data = new QueueData { Callback = OnHistoryGetPage };
      //---
      data.Data = new DataOrderPage
                    {
                      Login = UserState.User.Login,
                      Offset = (m_Page - 1) * ORDERS_PAGE,
                      Total = ORDERS_PAGE,
                      From = 0,
                      To = Date.ConvertToUnixTimestamp(DateTime.Now)
                    };
      data.Name = "HistoryGetPage";
      //---
      QueueRequest.Add(data);
      }
    /// <summary>
    /// View html page, after all data get
    /// </summary>
    /// <returns></returns>
    public ActionResult IndexCompleted()
      {
      if(!UserState.IsLoggedIn)
        {
        return Http404NotFound("");
        }
      //---
      ViewOrders viewOrders = new ViewOrders { Orders = m_Orders,CurrentPage = m_Page };
      return View("Index",viewOrders);
      }
    /// <summary>
    /// Get data of positions
    /// </summary>
    /// <param name="result">result code from MT server</param>
    /// <param name="data">result datat from MT server</param>
    public void OnHistoryGetPage(MTRetCode result,object data)
      {
      if(result != MTRetCode.MT_RET_OK)
        {
        Log.Write(LogType.Error,"OnOrdersGetPage","orders did not get from MetaTrader 5 server");
        }
      else
        {
        m_Orders = data as List<MTOrder>;
        if(m_Orders == null)
          {
          ViewData.ModelState.AddModelError("common","orders did not get from MetaTrader 5 server");
          return;
          }
        //---
        Log.Write(LogType.Error,"OnOrdersGetPage",string.Format("orders get total: {0}",m_Orders.Count));
        }
      //---
      AsyncManager.OutstandingOperations.Decrement();
      }
    }
  }
