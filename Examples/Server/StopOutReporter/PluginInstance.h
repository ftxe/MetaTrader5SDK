//+------------------------------------------------------------------+
//|                                          StopOut Reporter Plugin |
//|                             Copyright 2000-2023, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Plugin instance class                                            |
//+------------------------------------------------------------------+
class CPluginInstance : public IMTServerPlugin,IMTConPluginSink
  {
private:
   //--- ���������
   enum constants
     {
      STOP_OUT_PERIOD  =60*60*12,     // ������ ����������� ����������� (12 �����)
      BASE_VERSION     =100,          // ������ ����
      BASE_MAX         =65536,        // ������������ ������ ����
      SEND_PAUSE       =1000,         // ���������� �������� ������������� �������
      SEND_PAUSE_TOTAL =120,          // ����� ���������� �������� 
      STACK_SIZE_COMMON=262144,       // ��� �����
     };
   //--- ������ �������
   struct StopOutInfo
     {
      UINT64            login;        // �����
      INT64             datetime;     // ����� SO ��� MC
      double            margin;       // �����
      double            equity;       // ������
      double            level;        // ������� �����
      double            value;        // �������� � ��������� ��� � �������
      double            limit;        // ����������� �������� ��� SO ��� MC
      bool              mc_flag;      // ���� ����������� MC
      bool              mc_send_flag; // ���� �������� MC
      bool              so_flag;      // ���� ����������� SO
      bool              so_send_flag; // ���� �������� SO
     };
   //--- ������ ������
   typedef TMTArray<StopOutInfo> StopOutInfoArray;

private:
   HMODULE           m_module;
   IMTServerAPI*     m_api;
   IMTConPlugin*     m_config;
   //--- ���� ����-�����
   StopOutInfoArray  m_stopouts;
   //--- �������
   CMTMemPack        m_templ_mc;       // ������ ��� ������-�����
   CMTMemPack        m_templ_so;       // ������ ��� ������-�����
   //--- ������
   volatile LONG     m_terminate;
   CMTThread         m_check_thread;
   CMTThread         m_send_thread;    // ����� ������� �����
   CMTProcess        m_send_process;   // ������� ������� �����
   CMTFile           m_send_config;    // ���� ������������ �������
   //--- ���������
   CMTStr256         m_path_data;
   CMTStr256         m_path_root;
   CMTStr256         m_group_mask;
   INT64             m_check_period;
   INT64             m_notify_pause;
   CMTStr256         m_notify_from;
   CMTStr256         m_notify_copy;
   CMTStr256         m_notify_server;
   CMTStr256         m_notify_login;
   CMTStr256         m_notify_password;
   //--- ����� ������
   CMTMemPack        m_report;
   //--- ��������������� ����������
   IMTConGroup*      m_group;
   IMTAccount*       m_account;
   IMTUser*          m_user;

public:
                     CPluginInstance(HMODULE hmodule);
                    ~CPluginInstance(void);
   //--- IMTServerPlugin interface implementation
   virtual void      Release(void);
   virtual MTAPIRES  Start(IMTServerAPI* server);
   virtual MTAPIRES  Stop(void);
   //--- IMTConPluginSink interface implementation
   virtual void      OnPluginUpdate(const IMTConPlugin* plugin);

private:
   MTAPIRES          ParametersRead(void);
   bool              TemplatesRead(LPCWSTR filename,const UINT id_res,CMTMemPack& pack);
   //--- ���� ����-�����
   void              BaseRead(void);
   void              BaseSave(void);
   //--- �������� ������
   void              CheckStopout(void);
   void              CheckStopoutLogin(const IMTConGroup* group,const UINT64 login);
   //--- �������� � ������� �����������
   void              StopOutSend(void);
   bool              StopOutGenerate(CMTMemPack& templ,StopOutInfo& info);
   //--- ����
   static UINT __stdcall CheckThreadWrapper(void *param);
   void              CheckThread(void);
   //--- ����� �������
   static UINT __stdcall SendThreadWrapper(LPVOID param);
   void              SendThread(void);
   //--- ����������
   static int        SortByLogin(const void *left,const void *right);
   static int        SearchByLogin(const void *left,const void *right);
  };
//+------------------------------------------------------------------+