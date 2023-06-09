﻿//+------------------------------------------------------------------+
//|                        MetaTrader 5 API Manager for .NET Example |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
namespace BalanceExample.NET
  {
   using System;
   using System.Windows.Forms;
   //+------------------------------------------------------------------+
   //|                                                                  |
   //+------------------------------------------------------------------+
   static class BalanceExampleApp
     {
      //+------------------------------------------------------------------+
      //|                                                                  |
      //+------------------------------------------------------------------+
      [STAThread]
      static void Main()
        {
         try
           {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new CBalanceExampleDlg());
           }
         catch(Exception ex)
           {
            Console.WriteLine(ex.ToString());
           }
        }
     }
  }
//+------------------------------------------------------------------+
