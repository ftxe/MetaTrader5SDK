//+------------------------------------------------------------------+
//|                                                     SimpleDealer |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "Dealer.h"
#include "stringtools.h"

//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CDealer::CDealer(void)
  : m_manager(NULL), m_login(0), m_request(NULL), m_confirm(NULL),
  m_stop_flag(false), m_thread_dealer(NULL), m_connected(FALSE), m_event_request(NULL)
{
  m_server[0] = L'\0';
  m_password[0] = L'\0';
}
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CDealer::~CDealer(void)
{
  Shutdown();
}
//+------------------------------------------------------------------+
//| Dealer starting                                                  |
//+------------------------------------------------------------------+
bool CDealer::Start(LPCWSTR server, UINT64 login, LPCWSTR password)
{
  DWORD id = 0;
  //--- check arguments
  if (!server || login == 0 || !password)
  {
    wprintf_s(L"Dealer: starting failed with bad arguments\n");
    return(false);
  }
  //--- check if dealer is started already
  if (m_thread_dealer != NULL)
  {
    GetExitCodeThread(m_thread_dealer, &id);
    //--- dealer thread is running
    if (id == STILL_ACTIVE) return(false);
    //--- close handle
    CloseHandle(m_thread_dealer);
    m_thread_dealer = NULL;
  }
  //--- initialize Manager API
  if (!Initialize())
    return(false);
  //--- save authorize info
  wcscpy_s(m_server, server);
  m_login = login;
  wcscpy_s(m_password, password);
  //--- start dealing thread
  m_stop_flag = false;
  m_connected = FALSE;
  if ((m_thread_dealer = (HANDLE)_beginthreadex(NULL, STACK_SIZE_COMMON, DealerWrapper, this, STACK_SIZE_PARAM_IS_A_RESERVATION, (UINT*)&id)) == NULL)
  {
    wprintf_s(L"Dealer: starting dealer thread failed\n");
    return(false);
  }
  //--- done
  //--- connect mysql
  m_sql.init("tcp://8.219.243.211:3306", "root", "news#test", "mc900-news-test");
  //--- done
  return(true);
}
//+------------------------------------------------------------------+
//| Dealer stopping                                                  |
//+------------------------------------------------------------------+
void CDealer::Stop(void)
{
  //--- disconnect, release, and unload
  Shutdown();
}
//+------------------------------------------------------------------+
//| Manager interface initialization                                 |
//+------------------------------------------------------------------+
bool CDealer::Initialize(void)
{
  MTAPIRES res = MT_RET_OK_NONE;
  UINT     version = 0;
  //--- initialize factory
  if ((res = m_factory.Initialize(NULL)) != MT_RET_OK)
  {
    wprintf_s(L"Dealer: loading manager API failed (%u)\n", res);
    return(false);
  }
  //--- check Manager API version
  if ((res = m_factory.Version(version)) != MT_RET_OK)
  {
    wprintf_s(L"Dealer: getting version failed (%u)\n", res);
    return(false);
  }
  wprintf(L"Dealer: Manager API version %u has been loaded\n", version);
  if (version < MTManagerAPIVersion)
  {
    wprintf_s(L"Dealer: wrong Manager API version, version %u required\n", MTManagerAPIVersion);
    return(false);
  }
  //--- create manager interface
  if ((res = m_factory.CreateManager(MTManagerAPIVersion, &m_manager)) != MT_RET_OK)
  {
    wprintf_s(L"Dealer: creating manager interface failed (%u)\n", res);
    return(false);
  }
  //--- subscribe for notifications
  if (m_manager->Subscribe(this) != MT_RET_OK)
    return(false);
  //--- subscribe for requests
  if (m_manager->RequestSubscribe(this) != MT_RET_OK)
    return(false);
  //--- subscribe for orders
  if (m_manager->OrderSubscribe(this) != MT_RET_OK)
    return(false);
  //--- subscribe for positions
  if (m_manager->PositionSubscribe(this) != MT_RET_OK)
    return(false);
  //--- subscribe for deals
  if (m_manager->DealSubscribe(this) != MT_RET_OK)
    return(false);
  //--- subscribe for users
  if (m_manager->UserSubscribe(this) != MT_RET_OK)
    return(false);
  //--- create request object
  if ((m_request = m_manager->RequestCreate()) == NULL)
  {
    wprintf_s(L"Dealer: creating request object failed\n");
    return(false);
  }
  //--- create confirmation object
  if ((m_confirm = m_manager->DealerConfirmCreate()) == NULL)
  {
    wprintf_s(L"Dealer: creating confirmation object failed\n");
    return(false);
  }
  //--- create requests event
  m_event_request = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (m_event_request == NULL)
  {
    wprintf_s(L"Dealer: creating request event failed (%u)\n", GetLastError());
    return(false);
  }
  //--- done

  m_user = m_manager->UserCreate();
  return(true);
}
//+------------------------------------------------------------------+
//| Manager API shutdown                                             |
//+------------------------------------------------------------------+
void CDealer::Shutdown()
{
  //--- wait for dealing thread exit
  if (m_thread_dealer)
  {
    //--- set thread stop flag
    m_stop_flag = true;
    //--- release dealer thread from waiting state
    if (WaitForSingleObject(m_event_request, 0) == WAIT_TIMEOUT)
      SetEvent(m_event_request);
    //--- wait for thread exit
    WaitForSingleObject(m_thread_dealer, INFINITE);
    CloseHandle(m_thread_dealer);
    m_thread_dealer = NULL;
  }
  //--- release request objects
  if (m_request)
  {
    m_request->Release();
    m_request = NULL;
  }
  //--- release confirmation objects
  if (m_confirm)
  {
    m_confirm->Release();
    m_confirm = NULL;
  }
  //--- if manager interface was created
  if (m_manager)
  {
    //--- unsubscribe from notifications
    m_manager->Unsubscribe(this);
    //--- unsubscribe from requests
    m_manager->RequestUnsubscribe(this);
    //--- unsubscribe from orders
    m_manager->OrderUnsubscribe(this);
    //--- unsubscribe from positions
    m_manager->PositionUnsubscribe(this);
    //--- unsubscribe from deals
    m_manager->DealUnsubscribe(this);
    //--- unsubscribe from users
    m_manager->UserUnsubscribe(this);
    //--- release manager interface
    m_manager->Release();
    m_manager = NULL;
  }
  //--- unload Manager API
  m_factory.Shutdown();
  //--- close requests event
  if (m_event_request)
  {
    CloseHandle(m_event_request);
    m_event_request = NULL;
  }
}
//+------------------------------------------------------------------+
//| Dealing thread wrapper                                           |
//+------------------------------------------------------------------+
UINT __stdcall CDealer::DealerWrapper(LPVOID param)
{
  //--- dealing thread
  if (param) ((CDealer*)param)->DealerFunc();
  //--- done
  return(0);
}
//+------------------------------------------------------------------+
//| Dealing thread function                                          |
//+------------------------------------------------------------------+
void CDealer::DealerFunc(void)
{
  //--- deal
  while (!m_stop_flag)
  {
    //--- connect to the server
    if (!InterlockedExchangeAdd(&m_connected, 0))
    {
      //--- connect manager to the server
      if (m_manager->Connect(m_server, m_login, m_password, NULL,
        IMTManagerAPI::PUMP_MODE_SYMBOLS |
        IMTManagerAPI::PUMP_MODE_GROUPS |
        IMTManagerAPI::PUMP_MODE_USERS |
        IMTManagerAPI::PUMP_MODE_ACTIVITY |
        IMTManagerAPI::PUMP_MODE_ORDERS |
        IMTManagerAPI::PUMP_MODE_POSITIONS,
        MT5_CONNECT_TIMEOUT) != MT_RET_OK)
      {
        Sleep(100);
        continue;
      }
      //--- start dealer
      if (m_manager->DealerStart() != MT_RET_OK)
      {
        Sleep(100);
        continue;
      }
      InterlockedExchange(&m_connected, TRUE);
    }
    //--- wait for request
    WaitForSingleObject(m_event_request, INFINITE);
    //--- check stop flag
    if (m_stop_flag)
      break;
    //--- get next request
    if (m_manager->DealerGet(m_request) == MT_RET_OK)
      DealerAnswer();
    else Sleep(100);
  }
  //--- stop dealer
  m_manager->DealerStop();
  //--- disconnect manager
  m_manager->Disconnect();
}
//+------------------------------------------------------------------+
//| Answer on request                                                |
//+------------------------------------------------------------------+
void CDealer::DealerAnswer(void)
{
  MTTickShort tick;
  MTAPISTR    str = L"";
  //--- print request
  m_request->Print(str);
  //--- clear confirmation
  m_confirm->Clear();
  //--- request id
  m_confirm->ID(m_request->ID());
  //--- get last tick for request symbol
  if (m_manager->TickLast(m_request->Symbol(), tick) != MT_RET_OK)
  {
    //--- ticks not found
    //--- select symbol
    m_manager->SelectedAdd(m_request->Symbol());
    //--- return request
    m_confirm->Retcode(MT_RET_REQUEST_RETURN);
    if (m_manager->DealerAnswer(m_confirm) == MT_RET_OK)
      m_manager->LoggerOut(MTLogOK, L"'%I64u': return %s for '%I64u'", m_login, str, m_request->Login());
    return;
  }
  //--- confirm request
  m_confirm->Retcode(MT_RET_REQUEST_DONE);
  m_confirm->Volume(m_request->Volume());
  if (m_request->Action() == IMTRequest::TA_MARKET)
    switch (m_request->Type())
    {
    case IMTOrder::OP_BUY:
      m_confirm->Price(tick.ask);
      break;
    case IMTOrder::OP_SELL:
      m_confirm->Price(tick.bid);
      break;
    }
  else
    m_confirm->Price(m_request->PriceOrder());
  m_confirm->TickBid(tick.bid);
  m_confirm->TickAsk(tick.ask);
  m_confirm->TickLast(tick.last);
  //--- send confirm
  if (m_manager->DealerAnswer(m_confirm) == MT_RET_OK)
    m_manager->LoggerOut(MTLogOK, L"'%I64u': confirm %s for '%I64u'", m_login, str, m_request->Login());
}
//+------------------------------------------------------------------+
//| Disconnect notification                                          |
//+------------------------------------------------------------------+
void CDealer::OnDisconnect(void)
{
  //--- need to reconnect
  InterlockedExchange(&m_connected, FALSE);
  //--- continue dealer thread
  if (WaitForSingleObject(m_event_request, 0) == WAIT_TIMEOUT)
    SetEvent(m_event_request);
}
//+------------------------------------------------------------------+
//| Request notifications                                            |
//+------------------------------------------------------------------+
void CDealer::NotifyRequest(void)
{
  //--- check for available requests
  if (m_manager->RequestTotal() > 0)
  {
    //--- request exists      
    if (WaitForSingleObject(m_event_request, 0) == WAIT_TIMEOUT)
      SetEvent(m_event_request);
  }
  else
  {
    //--- requests queue is empty
    if (WaitForSingleObject(m_event_request, 0) == WAIT_OBJECT_0)
      ResetEvent(m_event_request);
  }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnOrderAdd(const IMTOrder* order)
{
  if (order)
  {
    MTAPISTR str = L"";
    order->Print(str);
    m_manager->LoggerOut(MTLogOK, L"Order: %s has been added", str);
    auto User = order->Login();
    MTAPIRES res = m_manager->UserRequest(User, m_user);
    if (res != MT_RET_OK)
    {
      m_manager->LoggerOut(MTLogErr, L"UserRequest error (%u)", res);
    }
    auto Account = m_user->Account();
    auto Group = m_user->Group();
    auto Leverage = m_user->Leverage();
    auto Name = m_user->Name();
    auto FirstName = m_user->FirstName();
    auto LastName = m_user->LastName();
    auto Order = order->Order();
    auto ExternalID = order->ExternalID();
    auto Dealer = order->Dealer();
    auto Symbol = order->Symbol();
    auto Digits = order->Digits();
    auto DigitsCurrency = order->DigitsCurrency();
    auto ContractSize = order->ContractSize();
    auto State = order->State();
    auto Reason = order->Reason();
    auto TimeSetup = order->TimeSetup();
    auto TimeExpiration = order->TimeExpiration();
    auto Type = order->Type();
    auto TypeFill = order->TypeFill();
    auto TimeDone = order->TimeDone();
    auto priceOrder = order->PriceOrder();
    auto priceCurrent = order->PriceCurrent();
    auto volumeInitial = order->VolumeInitial();
    auto volumeCurrent = order->VolumeCurrent();
    auto PriceTrigger = order->PriceTrigger();
    auto PriceSL = order->PriceSL();
    auto PriceTP = order->PriceTP();
    auto PositionID = order->PositionID();
    auto Comment = order->Comment();
    auto ActivationMode = order->ActivationMode();
    auto ActivationTime = order->ActivationTime();
    auto ActivationPrice = order->ActivationPrice();
    auto ActivationFlags = order->ActivationFlags();
    m_manager->LoggerOut(MTLogOK, L"OnOrderAdd: User:%lld, Order:%lld, ExternalID:%s, Dealer:%lld, Symbol:%s, Digits:%d, DigitsCurrency:%d, "
      L"ContractSize:%lf, State:%d, Reason:%d, "
      L"TimeSetup:%lld, TimeExpiration:%lld, Type:%d, TypeFill:%d, TimeDone:%lld, priceOrder:%lf, priceCurrent:%lf, "
      L"volumeInitial:%d, volumeCurrent:%d, PriceTrigger:%lf, PriceSL:%lf, PriceTP:%lf, PositionID:%lld, Comment:%s, "
      L"ActivationMode:%d, ActivationTime:%lld, ActivationPrice:%lf, ActivationFlags:%d",
      User, Order, ExternalID, Dealer, Symbol, Digits, DigitsCurrency,
      ContractSize, State, Reason,
      TimeSetup, TimeExpiration, Type, TypeFill, TimeDone, priceOrder, priceCurrent,
      volumeInitial, volumeCurrent, PriceTrigger, PriceSL, PriceTP, PositionID, Comment,
      ActivationMode, ActivationTime, ActivationPrice, ActivationFlags);
    auto syncTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    bool ret_ = m_sql.insert("insert into mt5_order (sync_time, order_id, customer_id, customer_group, symbol_name, "
      "order_type, order_type_fill, time_expiration, time_setup, time_done, digits, digits_currency, contract_size, "
      "order_price, trigger_price, current_price, volume, order_stat, error_code, create_by, create_time, remark) "
      "values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",
      std::tuple<INT64, UINT64, UINT64, std::string, std::string, 
      UINT, UINT, INT64, INT64, INT64, UINT, UINT, double,
      double, double, double, UINT64, UINT, UINT, std::string, INT64, std::string>(
        syncTime, Order, User, WString2String(Group), WString2String(Symbol),
        Type, TypeFill, TimeExpiration, TimeSetup, TimeDone, Digits, DigitsCurrency, ContractSize, 
        priceOrder, PriceTrigger, priceCurrent, volumeCurrent, State, Reason, WString2String(Name), TimeSetup, WString2String(Comment)));

  }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnOrderUpdate(const IMTOrder* order)
{
  if (order)
  {
    MTAPISTR str = L"";
    order->Print(str);
    m_manager->LoggerOut(MTLogOK, L"Order: %s has been updated", str);
    auto User = order->Login();
    MTAPIRES res = m_manager->UserRequest(User, m_user);
    if (res != MT_RET_OK)
    {
      m_manager->LoggerOut(MTLogErr, L"UserRequest error (%u)", res);
    }
    auto Account = m_user->Account();
    auto Group = m_user->Group();
    auto Leverage = m_user->Leverage();
    auto Name = m_user->Name();
    auto FirstName = m_user->FirstName();
    auto LastName = m_user->LastName();
    auto Order = order->Order();
    auto ExternalID = order->ExternalID();
    auto Dealer = order->Dealer();
    auto Symbol = order->Symbol();
    auto Digits = order->Digits();
    auto DigitsCurrency = order->DigitsCurrency();
    auto ContractSize = order->ContractSize();
    auto State = order->State();
    auto Reason = order->Reason();
    auto TimeSetup = order->TimeSetup();
    auto TimeExpiration = order->TimeExpiration();
    auto Type = order->Type();
    auto TypeFill = order->TypeFill();
    auto TimeDone = order->TimeDone();
    auto priceOrder = order->PriceOrder();
    auto priceCurrent = order->PriceCurrent();
    auto volumeInitial = order->VolumeInitial();
    auto volumeCurrent = order->VolumeCurrent();
    auto PriceTrigger = order->PriceTrigger();
    auto PriceSL = order->PriceSL();
    auto PriceTP = order->PriceTP();
    auto PositionID = order->PositionID();
    auto Comment = order->Comment();
    auto ActivationMode = order->ActivationMode();
    auto ActivationTime = order->ActivationTime();
    auto ActivationPrice = order->ActivationPrice();
    auto ActivationFlags = order->ActivationFlags();
    m_manager->LoggerOut(MTLogOK, L"OnOrderUpdate: User:%lld, Order:%lld, ExternalID:%s, Dealer:%lld, Symbol:%s, Digits:%d, DigitsCurrency:%d, "
      L"ContractSize:%lf, State:%d, Reason:%d, "
      L"TimeSetup:%lld, TimeExpiration:%lld, Type:%d, TypeFill:%d, TimeDone:%lld, priceOrder:%lf, priceCurrent:%lf, "
      L"volumeInitial:%d, volumeCurrent:%d, PriceTrigger:%lf, PriceSL:%lf, PriceTP:%lf, PositionID:%lld, Comment:%s, "
      L"ActivationMode:%d, ActivationTime:%lld, ActivationPrice:%lf, ActivationFlags:%d",
      User, Order, ExternalID, Dealer, Symbol, Digits, DigitsCurrency,
      ContractSize, State, Reason,
      TimeSetup, TimeExpiration, Type, TypeFill, TimeDone, priceOrder, priceCurrent,
      volumeInitial, volumeCurrent, PriceTrigger, PriceSL, PriceTP, PositionID, Comment,
      ActivationMode, ActivationTime, ActivationPrice, ActivationFlags);
  }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnOrderDelete(const IMTOrder* order)
{
  if (order)
  {
    MTAPISTR str = L"";
    order->Print(str);
    m_manager->LoggerOut(MTLogOK, L"Order: %s has been deleted", str);
    auto User = order->Login();
    MTAPIRES res = m_manager->UserRequest(User, m_user);
    if (res != MT_RET_OK)
    {
      m_manager->LoggerOut(MTLogErr, L"UserRequest error (%u)", res);
    }
    auto Account = m_user->Account();
    auto Group = m_user->Group();
    auto Leverage = m_user->Leverage();
    auto Name = m_user->Name();
    auto FirstName = m_user->FirstName();
    auto LastName = m_user->LastName();
    auto Order = order->Order();
    auto ExternalID = order->ExternalID();
    auto Dealer = order->Dealer();
    auto Symbol = order->Symbol();
    auto Digits = order->Digits();
    auto DigitsCurrency = order->DigitsCurrency();
    auto ContractSize = order->ContractSize();
    auto State = order->State();
    auto Reason = order->Reason();

    auto TimeSetup = order->TimeSetup();
    auto TimeExpiration = order->TimeExpiration();
    auto Type = order->Type();
    auto TypeFill = order->TypeFill();
    auto TimeDone = order->TimeDone();
    auto priceOrder = order->PriceOrder();
    auto priceCurrent = order->PriceCurrent();
    auto volumeInitial = order->VolumeInitial();
    auto volumeCurrent = order->VolumeCurrent();
    auto PriceTrigger = order->PriceTrigger();
    auto PriceSL = order->PriceSL();
    auto PriceTP = order->PriceTP();
    auto PositionID = order->PositionID();
    auto Comment = order->Comment();
    auto ActivationMode = order->ActivationMode();
    auto ActivationTime = order->ActivationTime();
    auto ActivationPrice = order->ActivationPrice();
    auto ActivationFlags = order->ActivationFlags();
    m_manager->LoggerOut(MTLogOK, L"OnOrderUpdate: User:%lld, Order:%lld, ExternalID:%s, Dealer:%lld, Symbol:%s, Digits:%d, DigitsCurrency:%d, "
      L"ContractSize:%lf, State:%d, Reason:%d, "
      L"TimeSetup:%lld, TimeExpiration:%lld, Type:%d, TypeFill:%d, TimeDone:%lld, priceOrder:%lf, priceCurrent:%lf, "
      L"volumeInitial:%d, volumeCurrent:%d, PriceTrigger:%lf, PriceSL:%lf, PriceTP:%lf, PositionID:%lld, Comment:%s, "
      L"ActivationMode:%d, ActivationTime:%lld, ActivationPrice:%lf, ActivationFlags:%d",
      User, Order, ExternalID, Dealer, Symbol, Digits, DigitsCurrency,
      ContractSize, State, Reason,
      TimeSetup, TimeExpiration, Type, TypeFill, TimeDone, priceOrder, priceCurrent,
      volumeInitial, volumeCurrent, PriceTrigger, PriceSL, PriceTP, PositionID, Comment,
      ActivationMode, ActivationTime, ActivationPrice, ActivationFlags);
  }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnPositionAdd(const IMTPosition* position)
{
  if (position)
  {
    MTAPISTR str = L"";
    position->Print(str);
    m_manager->LoggerOut(MTLogOK, L"Position: %s has been added", str);
    auto User = position->Login();
    m_user->Clear();
    MTAPIRES res = m_manager->UserRequest(User, m_user);
    if (res != MT_RET_OK)
    {
      m_manager->LoggerOut(MTLogErr, L"UserRequest error (%u)", res);
    }
    auto Account = m_user->Account();
    auto Group = m_user->Group();
    auto Leverage = m_user->Leverage();
    auto Name = m_user->Name();
    auto FirstName = m_user->FirstName();
    auto LastName = m_user->LastName();

    auto Symbol = position->Symbol();
    auto Action = position->Action();
    auto Digits = position->Digits();
    auto DigitsCurrency = position->DigitsCurrency();
    auto ContractSize = position->ContractSize();
    auto TimeCreate = position->TimeCreate();
    auto TimeUpdate = position->TimeUpdate();
    auto PriceOpen = position->PriceOpen();
    auto PriceCurrent = position->PriceCurrent();
    auto PriceSL = position->PriceSL();
    auto PriceTP = position->PriceTP();
    auto Volume = position->Volume();
    auto Profit = position->Profit();
    auto Storage = position->Storage();
    auto ObsoleteValue = position->ObsoleteValue();
    auto RateProfit = position->RateProfit();
    auto RateMargin = position->RateMargin();
    auto Comment = position->Comment();
    auto Reason = position->Reason();
    auto Position = position->Position();
    auto ActivationMode = position->ActivationMode();
    auto ActivationTime = position->ActivationTime();
    auto ActivationPrice = position->ActivationPrice();
    auto ActivationFlags = position->ActivationFlags();
    auto Dealer = position->Dealer();
    m_manager->LoggerOut(MTLogOK, L"OnPositionAdd: Account:%s, Group:%s, Leverage:%d, Name:%s, FirstName:%s, LastName:%s, User:%lld, "
      L"Symbol:%s, Action:%d, Digits:%d, DigitsCurrency:%d, ContractSize:%lf, TimeCreate:%lld, TimeUpdate:%lld "
      L"PriceOpen:%lf, PriceCurrent:%lf, PriceSL:%lf, PriceTP:%lf, Volume:%d, Profit:%lf, Storage:%lf, ObsoleteValue:%lf, "
      L"RateProfit:%lf, RateMargin:%lf, Comment:%s, Reason:%d, Position:%lld, "
      L"ActivationMode:%d, ActivationTime:%lld, ActivationPrice:%lf, ActivationFlags:%d",
      Account, Group, Leverage, Name, FirstName, LastName, User,
      Symbol, Action, Digits, DigitsCurrency, ContractSize, TimeCreate, TimeUpdate,
      PriceOpen, PriceCurrent, PriceSL, PriceTP, Volume, Profit, Storage, ObsoleteValue,
      RateProfit, RateMargin, Comment, Reason, Position,
      ActivationMode, ActivationTime, ActivationPrice, ActivationFlags);
    auto syncTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    bool ret_ = m_sql.insert("insert into mt5_position (sync_time, position_id, customer_id, customer_group, symbol_name, "
      "direction, digits, digits_currency, contract_size, open_price, cost_price, volume, deal_id, create_by, create_time) "
      "values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",
      std::tuple<INT64, UINT64, UINT64, std::string, std::string,
      UINT, UINT, UINT, UINT, double, double, UINT64, UINT64, std::string, INT64>(
        syncTime, Position, User, WString2String(Group), WString2String(Symbol),
        Action, Digits, DigitsCurrency, ContractSize, PriceOpen, PriceCurrent, Volume, Dealer, WString2String(Name), TimeCreate));
  }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnPositionUpdate(const IMTPosition* position)
{
  if (position)
  {
    MTAPISTR str = L"";
    position->Print(str);
    m_manager->LoggerOut(MTLogOK, L"Position: %s has been updated", str);
    auto User = position->Login();
    m_user->Clear();
    MTAPIRES res = m_manager->UserRequest(User, m_user);
    if (res != MT_RET_OK)
    {
      m_manager->LoggerOut(MTLogErr, L"UserRequest error (%u)", res);
    }
    auto Account = m_user->Account();
    auto Group = m_user->Group();
    auto Leverage = m_user->Leverage();
    auto Name = m_user->Name();
    auto FirstName = m_user->FirstName();
    auto LastName = m_user->LastName();

    auto Symbol = position->Symbol();
    auto Action = position->Action();
    auto Digits = position->Digits();
    auto DigitsCurrency = position->DigitsCurrency();
    auto ContractSize = position->ContractSize();
    auto TimeCreate = position->TimeCreate();
    auto TimeUpdate = position->TimeUpdate();
    auto PriceOpen = position->PriceOpen();
    auto PriceCurrent = position->PriceCurrent();
    auto PriceSL = position->PriceSL();
    auto PriceTP = position->PriceTP();
    auto Volume = position->Volume();
    auto Profit = position->Profit();
    auto Storage = position->Storage();
    auto ObsoleteValue = position->ObsoleteValue();
    auto RateProfit = position->RateProfit();
    auto RateMargin = position->RateMargin();
    auto Comment = position->Comment();
    auto Reason = position->Reason();
    auto Position = position->Position();
    auto ActivationMode = position->ActivationMode();
    auto ActivationTime = position->ActivationTime();
    auto ActivationPrice = position->ActivationPrice();
    auto ActivationFlags = position->ActivationFlags();
    m_manager->LoggerOut(MTLogOK, L"OnPositionUpdate: Account:%s, Group:%s, Leverage:%d, Name:%s, FirstName:%s, LastName:%s, User:%lld, "
      L"Symbol:%s, Action:%d, Digits:%d, DigitsCurrency:%d, ContractSize:%lf, TimeCreate:%lld, TimeUpdate:%lld "
      L"PriceOpen:%lf, PriceCurrent:%lf, PriceSL:%lf, PriceTP:%lf, Volume:%d, Profit:%lf, Storage:%lf, ObsoleteValue:%lf, "
      L"RateProfit:%lf, RateMargin:%lf, Comment:%s, Reason:%d, Position:%lld, "
      L"ActivationMode:%d, ActivationTime:%lld, ActivationPrice:%lf, ActivationFlags:%d",
      Account, Group, Leverage, Name, FirstName, LastName, User,
      Symbol, Action, Digits, DigitsCurrency, ContractSize, TimeCreate, TimeUpdate,
      PriceOpen, PriceCurrent, PriceSL, PriceTP, Volume, Profit, Storage, ObsoleteValue,
      RateProfit, RateMargin, Comment, Reason, Position,
      ActivationMode, ActivationTime, ActivationPrice, ActivationFlags);
  }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnPositionDelete(const IMTPosition* position)
{
  if (position)
  {
    MTAPISTR str = L"";
    position->Print(str);
    m_manager->LoggerOut(MTLogOK, L"Position: %s has been deleted", str);
    auto User = position->Login();
    m_user->Clear();
    MTAPIRES res = m_manager->UserRequest(User, m_user);
    if (res != MT_RET_OK)
    {
      m_manager->LoggerOut(MTLogErr, L"UserRequest error (%u)", res);
    }
    auto Account = m_user->Account();
    auto Group = m_user->Group();
    auto Leverage = m_user->Leverage();
    auto Name = m_user->Name();
    auto FirstName = m_user->FirstName();
    auto LastName = m_user->LastName();

    auto Symbol = position->Symbol();
    auto Action = position->Action();
    auto Digits = position->Digits();
    auto DigitsCurrency = position->DigitsCurrency();
    auto ContractSize = position->ContractSize();
    auto TimeCreate = position->TimeCreate();
    auto TimeUpdate = position->TimeUpdate();
    auto PriceOpen = position->PriceOpen();
    auto PriceCurrent = position->PriceCurrent();
    auto PriceSL = position->PriceSL();
    auto PriceTP = position->PriceTP();
    auto Volume = position->Volume();
    auto Profit = position->Profit();
    auto Storage = position->Storage();
    auto ObsoleteValue = position->ObsoleteValue();
    auto RateProfit = position->RateProfit();
    auto RateMargin = position->RateMargin();
    auto Comment = position->Comment();
    auto Reason = position->Reason();
    auto Position = position->Position();
    auto ActivationMode = position->ActivationMode();
    auto ActivationTime = position->ActivationTime();
    auto ActivationPrice = position->ActivationPrice();
    auto ActivationFlags = position->ActivationFlags();
    m_manager->LoggerOut(MTLogOK, L"OnPositionDelete: Account:%s, Group:%s, Leverage:%d, Name:%s, FirstName:%s, LastName:%s, User:%lld, "
      L"Symbol:%s, Action:%d, Digits:%d, DigitsCurrency:%d, ContractSize:%lf, TimeCreate:%lld, TimeUpdate:%lld "
      L"PriceOpen:%lf, PriceCurrent:%lf, PriceSL:%lf, PriceTP:%lf, Volume:%d, Profit:%lf, Storage:%lf, ObsoleteValue:%lf, "
      L"RateProfit:%lf, RateMargin:%lf, Comment:%s, Reason:%d, Position:%lld, "
      L"ActivationMode:%d, ActivationTime:%lld, ActivationPrice:%lf, ActivationFlags:%d",
      Account, Group, Leverage, Name, FirstName, LastName, User,
      Symbol, Action, Digits, DigitsCurrency, ContractSize, TimeCreate, TimeUpdate,
      PriceOpen, PriceCurrent, PriceSL, PriceTP, Volume, Profit, Storage, ObsoleteValue,
      RateProfit, RateMargin, Comment, Reason, Position,
      ActivationMode, ActivationTime, ActivationPrice, ActivationFlags);
  }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnDealAdd(const IMTDeal* deal)
{
  if (deal)
  {
    MTAPISTR str = L"";
    deal->Print(str);
    m_manager->LoggerOut(MTLogOK, L"Deal: %s has been added", str);
    auto User = deal->Login();
    m_user->Clear();
    MTAPIRES res = m_manager->UserRequest(User, m_user);
    if (res != MT_RET_OK)
    {
      m_manager->LoggerOut(MTLogErr, L"UserRequest error (%u)", res);
    }
    auto Account = m_user->Account();
    auto Group = m_user->Group();
    auto Leverage = m_user->Leverage();
    auto Name = m_user->Name();
    auto FirstName = m_user->FirstName();
    auto LastName = m_user->LastName();
    auto Deal = deal->Deal();
    auto ExternalID = deal->ExternalID();
    auto Order = deal->Order();
    auto Action = deal->Action();
    auto Entry = deal->Entry();
    auto Symbol = deal->Symbol();
    auto Digits = deal->Digits();
    auto DigitsCurrency = deal->DigitsCurrency();
    auto ContractSize = deal->ContractSize();
    auto Time = deal->Time();
    auto Price = deal->Price();
    auto Volume = deal->Volume();
    auto Profit = deal->Profit();
    auto Storage = deal->Storage();
    auto Commission = deal->Commission();
    auto ObsoleteValue = deal->ObsoleteValue();
    auto RateProfit = deal->RateProfit();
    auto RateMargin = deal->RateMargin();
    auto ExpertID = deal->ExpertID();
    auto PositionID = deal->PositionID();
    auto Comment = deal->Comment();
    auto VolumeClosed = deal->VolumeClosed();
    auto TickValue = deal->TickValue();
    auto TickSize = deal->TickSize();
    auto TimeMsc = deal->TimeMsc();
    auto PriceSL = deal->PriceSL();
    auto PriceTP = deal->PriceTP();
    auto VolumeExt = deal->VolumeExt();
    auto Fee = deal->Fee();
    auto Value = deal->Value();
    auto MarketBid = deal->MarketBid();
    auto MarketAsk = deal->MarketAsk();
    auto MarketLast = deal->MarketLast();
    m_manager->LoggerOut(MTLogOK, L"OnDealAdd: Account:%s, Group:%s, Leverage:%d, Name:%s, FirstName:%s, LastName:%s, User:%lld, "
      L"Deal:%lld, ExternalID:%s, Order:%lld, Action:%d, Entry:%d, Symbol:%s, "
      L"Digits:%d, DigitsCurrency:%d, ContractSize:%lf, Time:%lld, Price:%lf, Volume:%d, Profit:%lf, Storage:%lf, Commission:%lf, ObsoleteValue:%lf, "
      L"RateProfit:%lf, RateMargin:%lf, ExpertID:%lld, PositionID:%lld, Comment:%s, VolumeClosed:%lld, TickValue:%lf, TickSize:%lf, "
      L"TimeMsc:%lld, PriceSL:%lf, PriceTP:%lf, VolumeExt:%lld, Fee:%lf, Value:%lf, MarketBid:%lf, MarketAsk:%lf, MarketLast:%lf."
      L"Reason:%d, Position:%lld"
      L"ActivationMode:%d, ActivationTime:%lld, ActivationPrice:%lf, ActivationFlags:%d",
      Account, Group, Leverage, Name, FirstName, LastName, User,
      Deal, ExternalID, Order, Action, Entry, Symbol,
      Digits, DigitsCurrency, ContractSize, Time, Price, Volume, Profit, Storage, Commission, ObsoleteValue,
      RateProfit, RateMargin, ExpertID, PositionID, Comment, VolumeClosed, TickValue, TickSize,
      TimeMsc, PriceSL, PriceTP, VolumeExt, Fee, Value, MarketBid, MarketAsk, MarketLast);
    auto syncTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    bool ret_ = m_sql.insert("insert into mt5_deal (sync_time, order_id, deal_id, customer_id, customer_group, symbol_name, direction, "
      "digits, digits_currency, contract_size, execute_price, execute_num, fee, pnl, execute_time, position_id, remark, create_by, create_time) "
      "values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",
      std::tuple<INT64, UINT64, UINT64, UINT64, std::string, std::string, UINT, UINT, UINT, double, double, UINT64, 
      double, double, INT64, UINT64, std::string, std::string, INT64>(
        syncTime, Order, Deal, User, WString2String(Group), WString2String(Symbol), Action, Digits, DigitsCurrency, ContractSize, Price, Volume,
        Fee, Profit, TimeMsc, PositionID, WString2String(Comment), WString2String(Name), Time));
  }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnDealUpdate(const IMTDeal* deal)
{
  if (deal)
  {
    MTAPISTR str = L"";
    deal->Print(str);
    m_manager->LoggerOut(MTLogOK, L"Deal: %s has been updated", str);
    auto User = deal->Login();
    m_user->Clear();
    MTAPIRES res = m_manager->UserRequest(User, m_user);
    if (res != MT_RET_OK)
    {
      m_manager->LoggerOut(MTLogErr, L"UserRequest error (%u)", res);
    }
    auto Account = m_user->Account();
    auto Group = m_user->Group();
    auto Leverage = m_user->Leverage();
    auto Name = m_user->Name();
    auto FirstName = m_user->FirstName();
    auto LastName = m_user->LastName();
    auto Deal = deal->Deal();
    auto ExternalID = deal->ExternalID();
    auto Order = deal->Order();
    auto Action = deal->Action();
    auto Entry = deal->Entry();
    auto Digits = deal->Digits();
    auto DigitsCurrency = deal->DigitsCurrency();
    auto ContractSize = deal->ContractSize();
    auto Time = deal->Time();
    auto Symbol = deal->Symbol();
    auto Price = deal->Price();
    auto Volume = deal->Volume();
    auto Profit = deal->Profit();
    auto Storage = deal->Storage();
    auto Commission = deal->Commission();
    auto ObsoleteValue = deal->ObsoleteValue();
    auto RateProfit = deal->RateProfit();
    auto RateMargin = deal->RateMargin();
    auto ExpertID = deal->ExpertID();
    auto PositionID = deal->PositionID();
    auto Comment = deal->Comment();
    auto VolumeClosed = deal->VolumeClosed();
    auto TickValue = deal->TickValue();
    auto TickSize = deal->TickSize();
    auto TimeMsc = deal->TimeMsc();
    auto PriceSL = deal->PriceSL();
    auto PriceTP = deal->PriceTP();
    auto VolumeExt = deal->VolumeExt();
    auto Fee = deal->Fee();
    auto Value = deal->Value();
    auto MarketBid = deal->MarketBid();
    auto MarketAsk = deal->MarketAsk();
    auto MarketLast = deal->MarketLast();
    m_manager->LoggerOut(MTLogOK, L"OnDealUpdate: Account:%s, Group:%s, Leverage:%d, Name:%s, FirstName:%s, LastName:%s, User:%lld, "
      L"Deal:%lld, ExternalID:%s, Order:%lld, Action:%d, Entry:%d, Symbol:%s, "
      L"Digits:%d, DigitsCurrency:%d, ContractSize:%lf, Time:%lld, Price:%lf, Volume:%d, Profit:%lf, Storage:%lf, Commission:%lf, ObsoleteValue:%lf, "
      L"RateProfit:%lf, RateMargin:%lf, ExpertID:%lld, PositionID:%lld, Comment:%s, VolumeClosed:%lld, TickValue:%lf, TickSize:%lf, "
      L"TimeMsc:%lld, PriceSL:%lf, PriceTP:%lf, VolumeExt:%lld, Fee:%lf, Value:%lf, MarketBid:%lf, MarketAsk:%lf, MarketLast:%lf."
      L"Reason:%d, Position:%lld"
      L"ActivationMode:%d, ActivationTime:%lld, ActivationPrice:%lf, ActivationFlags:%d",
      Account, Group, Leverage, Name, FirstName, LastName, User,
      Deal, ExternalID, Order, Action, Entry, Symbol,
      Digits, DigitsCurrency, ContractSize, Time, Price, Volume, Profit, Storage, Commission, ObsoleteValue,
      RateProfit, RateMargin, ExpertID, PositionID, Comment, VolumeClosed, TickValue, TickSize,
      TimeMsc, PriceSL, PriceTP, VolumeExt, Fee, Value, MarketBid, MarketAsk, MarketLast);
  }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnDealDelete(const IMTDeal* deal)
{
  if (deal)
  {
    MTAPISTR str = L"";
    deal->Print(str);
    m_manager->LoggerOut(MTLogOK, L"Deal: %s has been deleted", str);
    auto User = deal->Login();
    m_user->Clear();
    MTAPIRES res = m_manager->UserRequest(User, m_user);
    if (res != MT_RET_OK)
    {
      m_manager->LoggerOut(MTLogErr, L"UserRequest error (%u)", res);
    }
    auto Account = m_user->Account();
    auto Group = m_user->Group();
    auto Leverage = m_user->Leverage();
    auto Name = m_user->Name();
    auto FirstName = m_user->FirstName();
    auto LastName = m_user->LastName();
    auto Deal = deal->Deal();
    auto ExternalID = deal->ExternalID();
    auto Order = deal->Order();
    auto Action = deal->Action();
    auto Entry = deal->Entry();
    auto Digits = deal->Digits();
    auto DigitsCurrency = deal->DigitsCurrency();
    auto ContractSize = deal->ContractSize();
    auto Time = deal->Time();
    auto Symbol = deal->Symbol();
    auto Price = deal->Price();
    auto Volume = deal->Volume();
    auto Profit = deal->Profit();
    auto Storage = deal->Storage();
    auto Commission = deal->Commission();
    auto ObsoleteValue = deal->ObsoleteValue();
    auto RateProfit = deal->RateProfit();
    auto RateMargin = deal->RateMargin();
    auto ExpertID = deal->ExpertID();
    auto PositionID = deal->PositionID();
    auto Comment = deal->Comment();
    auto VolumeClosed = deal->VolumeClosed();
    auto TickValue = deal->TickValue();
    auto TickSize = deal->TickSize();
    auto TimeMsc = deal->TimeMsc();
    auto PriceSL = deal->PriceSL();
    auto PriceTP = deal->PriceTP();
    auto VolumeExt = deal->VolumeExt();
    auto Fee = deal->Fee();
    auto Value = deal->Value();
    auto MarketBid = deal->MarketBid();
    auto MarketAsk = deal->MarketAsk();
    auto MarketLast = deal->MarketLast();
    m_manager->LoggerOut(MTLogOK, L"OnDealDelete: Account:%s, Group:%s, Leverage:%d, Name:%s, FirstName:%s, LastName:%s, User:%lld, "
      L"Deal:%lld, ExternalID:%s, Order:%lld, Action:%d, Entry:%d, Symbol:%s, "
      L"Digits:%d, DigitsCurrency:%d, ContractSize:%lf, Time:%lld, Price:%lf, Volume:%d, Profit:%lf, Storage:%lf, Commission:%lf, ObsoleteValue:%lf, "
      L"RateProfit:%lf, RateMargin:%lf, ExpertID:%lld, PositionID:%lld, Comment:%s, VolumeClosed:%lld, TickValue:%lf, TickSize:%lf, "
      L"TimeMsc:%lld, PriceSL:%lf, PriceTP:%lf, VolumeExt:%lld, Fee:%lf, Value:%lf, MarketBid:%lf, MarketAsk:%lf, MarketLast:%lf."
      L"Reason:%d, Position:%lld"
      L"ActivationMode:%d, ActivationTime:%lld, ActivationPrice:%lf, ActivationFlags:%d",
      Account, Group, Leverage, Name, FirstName, LastName, User,
      Deal, ExternalID, Order, Action, Entry, Symbol,
      Digits, DigitsCurrency, ContractSize, Time, Price, Volume, Profit, Storage, Commission, ObsoleteValue,
      RateProfit, RateMargin, ExpertID, PositionID, Comment, VolumeClosed, TickValue, TickSize,
      TimeMsc, PriceSL, PriceTP, VolumeExt, Fee, Value, MarketBid, MarketAsk, MarketLast);
  }
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnUserUpdate(const IMTUser* user)
{
  if (user)
    m_manager->LoggerOut(MTLogOK, L"User: '%I64u' update", user->Login());
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnUserLogin(LPCWSTR ip, const IMTUser* user, const UINT type)
{
  if (user)
    m_manager->LoggerOut(MTLogOK, L"User: '%I64u' login", user->Login());
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CDealer::OnUserLogout(LPCWSTR ip, const IMTUser* user, const UINT type)
{
  if (user)
    m_manager->LoggerOut(MTLogOK, L"User: '%I64u' logout", user->Login());
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
