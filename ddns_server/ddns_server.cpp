//--------------------------------------------------------------------------------------
// Copyright (c) AcgDev
// https://www.AcgDev.com/
//--------------------------------------------------------------------------------------

#include "../nnn/Src/nnnSocketServer/nnnSocketServer.h"

#include "../Common/Common.h"

#include "Config/Config.h"
#include "Session_KeyIV/Session_KeyIV.h"
#include "Log/Log.h"
#include "Socket/Socket.h"
#include "Packet/packet.h"
#include "ddns_server.h"

namespace DDNS_Server
{

std::atomic<es_State>	g_running_state	= es_State::Stopped;	// ����������״̬

/*==============================================================
 * ��ʾ LOGO
 * display_logo()
 *==============================================================*/
static void display_logo()
{
	/*
+---------------------------------------------------------------------------------------------------------------+
|                                                AcgDev presents                                                |
|  _____  _____  _   _  _____                      _           _     _          _____                           |
| |  __ \|  __ \| \ | |/ ____|                    | |         | |   | |        / ____|                          |
| | |  | | |  | |  \| | (___ ______ __ _  ___   __| | __ _  __| | __| |_   _  | (___   ___ _ ____   _____ _ __  |
| | |  | | |  | | . ` |\___ \______/ _` |/ _ \ / _` |/ _` |/ _` |/ _` | | | |  \___ \ / _ \ '__\ \ / / _ \ '__| |
| | |__| | |__| | |\  |____) |    | (_| | (_) | (_| | (_| | (_| | (_| | |_| |  ____) |  __/ |   \ V /  __/ |    |
| |_____/|_____/|_| \_|_____/      \__, |\___/ \__,_|\__,_|\__,_|\__,_|\__, | |_____/ \___|_|    \_/ \___|_|    |
|                                   __/ |                               __/ |                                   |
|                                  |___/                               |___/                                    |
|                                                 www.AcgDev.com                                                |
+---------------------------------------------------------------------------------------------------------------+
	*/

	Log::ShowMessage("+---------------------------------------------------------------------------------------------------------------+\n");
	Log::ShowMessage("|                                                AcgDev presents                                                |\n");
	Log::ShowMessage("|  _____  _____  _   _  _____                      _           _     _          _____                           |\n");
	Log::ShowMessage("| |  __ \\|  __ \\| \\ | |/ ____|                    | |         | |   | |        / ____|                          |\n");
	Log::ShowMessage("| | |  | | |  | |  \\| | (___ ______ __ _  ___   __| | __ _  __| | __| |_   _  | (___   ___ _ ____   _____ _ __  |\n");
	Log::ShowMessage("| | |  | | |  | | . ` |\\___ \\______/ _` |/ _ \\ / _` |/ _` |/ _` |/ _` | | | |  \\___ \\ / _ \\ '__\\ \\ / / _ \\ '__| |\n");
	Log::ShowMessage("| | |__| | |__| | |\\  |____) |    | (_| | (_) | (_| | (_| | (_| | (_| | |_| |  ____) |  __/ |   \\ V /  __/ |    |\n");
	Log::ShowMessage("| |_____/|_____/|_| \\_|_____/      \\__, |\\___/ \\__,_|\\__,_|\\__,_|\\__,_|\\__, | |_____/ \\___|_|    \\_/ \\___|_|    |\n");
	Log::ShowMessage("|                                   __/ |                               __/ |                                   |\n");
	Log::ShowMessage("|                                  |___/                               |___/                                    |\n");
	Log::ShowMessage("|                                                 www.AcgDev.com                                                |\n");
	Log::ShowMessage("+---------------------------------------------------------------------------------------------------------------+\n");
}


/*==============================================================
 * ��ʼ��/����
 * DoInit()
 * DoFinal()
 *==============================================================*/
HRESULT DoInit()
{
	HRESULT hr;

	V_RETURN( Log::DoInit() );

	// ��ʼ�� Config
	V_RETURN( Config::DoInit() );

	display_logo();

	// ��ʼ��
	V_RETURN( Session_KeyIV::DoInit() );

	V_RETURN( Config::g_config->Read_Config(Config::g_config->m_config_file) );
	if(FAILED(hr))
		return hr;

	// ��ʼ�� Socket
	V_RETURN( Socket::DoInit() );

	// ��ʼ�� ddns_server_CLR
	ddns_server_CLR::DoInit();

	return S_OK;
}
//--------------------------------------------------
HRESULT DoFinal()
{
	HRESULT hr;

	// ��������
	V( Socket::DoFinal() );
	V( Session_KeyIV::DoFinal() );
	V( Log::DoFinal() );
	V( Config::DoFinal() );

	return S_OK;
}


/*==============================================================
 * ���з�����
 * run_server()
 *==============================================================*/
void run_server()
{
	HRESULT hr;

	// �򿪷�����
	V( Socket::Start() );
	if(FAILED(hr))
	{
		g_running_state = es_State::Stopped;
		return;
	}

	// ִ����ѭ��
	while(g_running_state != es_State::Exiting)
	{
		UINT64 tick = NNN::Time::tick64();

		//========== ִ�������Թ��� ==========(Start)
		if(g_running_state == es_State::Running)
			Socket::DoWork();

		Log::DoWork();

		bool is_busy = (NNN::Time::tick64() - tick >= Config::g_config->m_busy_time);
		if(!is_busy)
			NNN::Thread::Sleep(1);
		//========== ִ�������Թ��� ==========(End)
	}

	// �رշ�����
	V( Socket::Stop() );
}

}	// namespace DDNS_Server

/*==============================================================
 * ������
 * cleanup()
 *==============================================================*/
void cleanup()
{
	// ����
	DDNS_Server::DoFinal();
	NNN::DoFinal();
}


/*==============================================================
 * ������ڵ�
 * main()
 *==============================================================*/
int main()
{
#if (NNN_PLATFORM == NNN_PLATFORM_WIN32)
	// �ڴ�й©���
	NNN::Misc::MemoryLeakCheck();
#endif	// NNN_PLATFORM_WIN32

	NNN::DoInit();
	NNN::Misc::enable_core_dump(NNN::Misc::s_CoreDump_settings(L"ddns_server.dmp"));

	atexit(cleanup);

	// ��ʼ��
	if(FAILED(DDNS_Server::DoInit()))
		return EXIT_FAILURE;

	// ���� Server
	DDNS_Server::run_server();

	return 0;
}
