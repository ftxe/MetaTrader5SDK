//+------------------------------------------------------------------+
//|                               MetaTrader 5 Single Session Plugin |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Structure with description                                       |
//+------------------------------------------------------------------+
struct ClientSession
  {
   UINT              client_type;                           // connection type, enum IMTUser::EnUsersConnectionTypes
   wchar_t           client_ip[16];                         // IP
   UINT64            client_session_id;                     // session id
   wchar_t           client_computer_id[64];                // computer id
   UINT64            client_login;                          // client login
   wchar_t           client_group[64];                      // client group
   INT64             client_login_time;                     // time
   //--- filling of structure
   static bool       Fill(LPCWSTR ip_address,UINT64 session_id,LPCWSTR computer_id,const IMTUser *user,const UINT type,INT64 login_time,ClientSession &client_session);
  };
//+------------------------------------------------------------------+
//| Filling of structure                                             |
//+------------------------------------------------------------------+
inline bool ClientSession::Fill(LPCWSTR ip_address,
                                UINT64 session_id,
                                LPCWSTR computer_id,
                                const IMTUser *user,
                                const UINT type,
                                INT64 login_time,
                                ClientSession &client_session)
  {
//--- check
   if(!ip_address || ip_address[0]==L'\0' || !user || !computer_id || computer_id[0]==L'\0')
      return(false);
//--- clear the structure
   ZeroMemory(&client_session,sizeof(client_session));
//--- fill the fields
   client_session.client_type=type;
   CMTStr::Copy(client_session.client_ip,_countof(client_session.client_ip),ip_address);
   client_session.client_session_id=session_id;
   CMTStr::Copy(client_session.client_computer_id,_countof(client_session.client_computer_id),computer_id);
   client_session.client_login=user->Login();
   CMTStr::Copy(client_session.client_group,_countof(client_session.client_group),user->Group());
   client_session.client_login_time=login_time;
//--- successful
   return(true);
  }
//+------------------------------------------------------------------+
//| Array of structures                                              |
//+------------------------------------------------------------------+
typedef TMTArray<ClientSession> ClientSessionArray;
//+------------------------------------------------------------------+
//| Array of strings                                                 |
//+------------------------------------------------------------------+
typedef TMTArray<wchar_t[64],4> String64Array;
//+------------------------------------------------------------------+
