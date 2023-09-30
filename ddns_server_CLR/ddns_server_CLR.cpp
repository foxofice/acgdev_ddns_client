//--------------------------------------------------------------------------------------
// Copyright (c) AcgDev
// https://www.AcgDev.com/
//--------------------------------------------------------------------------------------
#include "../nnn/Src/nnnSocket/nnnSocket.h"
#include "../nnn/Src/nnnCLR/nnnCLR.h"

#include "ddns_server_CLR.h"

using namespace System;
using namespace System::Threading;
using namespace System::Collections::Generic;
using namespace System::Drawing;
using namespace System::Net;

namespace ddns_server_CLR
{

/*==============================================================
 * �����־
 * Add_Log()
 *==============================================================*/
void Add_Log(String ^txt, Color /*c*/)
{
	Console::WriteLine(txt);
}


/*==============================================================
 * ��ʼ��
 * DoInit()
 *==============================================================*/
void DoInit()
{
	/*
		�� c++ ��ִ�г������ .net ʱ��ServicePointManager.SecurityProtocol ���ܲ�û����ȷ�������ã�
		���� c++ ��Ŀ���ó� clr ֧��ʱ����Ŀ������ȷ���������ã������ܽ�����⣨������ֹ: δ�ܴ��� SSL/TLS ��ȫͨ��������
		���Ա���������д���
	*/
	ServicePointManager::SecurityProtocol		= SecurityProtocolType::Tls13;

	ServicePointManager::DefaultConnectionLimit	= 1000;

	ddns_lib::LIB::EVENTS::Event_On_AddLog += gcnew ddns_lib::LIB::EVENTS::e_Add_Log(&Add_Log);
}


/*==============================================================
 * ִ�и��¼�¼�ĺ���
 * update_records_func()
 *==============================================================*/
static void update_records_func(Object ^data)
{
	List<ddns_lib::c_Record^> ^records = (List<ddns_lib::c_Record^>^)data;
	ddns_lib::LIB::update_records(records, 15 * 1000);
}


/*==============================================================
 * ִ�и��� IP ��¼
 * update_records_step3_update_by_server()
 *==============================================================*/
void update_records_step3_update_by_server(	const char								*ip,
											const char								*Key,
											const char								*Secret,
											__inout std::vector<struct s_Record>	&records )
{
	List<ddns_lib::c_Record^> ^gc_records = gcnew List<ddns_lib::c_Record^>((int)records.size());

	for(const struct s_Record &record : records)
	{
		ddns_lib::c_Record ^gc_record = gcnew ddns_lib::c_Record();

		gc_record->m_name		= gcnew String(record.m_name);
		gc_record->m_domain		= gcnew String(record.m_domain);
		gc_record->m_TTL		= record.m_TTL;
		gc_record->m_user_idx	= record.m_user_idx;

		gc_record->m_ip			= gcnew String(ip);
		gc_record->m_Key		= gcnew String(Key);
		gc_record->m_Secret		= gcnew String(Secret);

		gc_records->Add(gc_record);
	}	// for

	ParameterizedThreadStart	^pts	= gcnew ParameterizedThreadStart(&update_records_func);
	Thread						^th		= gcnew Thread(pts);

	th->Start(gc_records);
	th->Join();

	for(int i=0; i<gc_records->Count; ++i)
	{
		ddns_lib::c_Record	^gc_record	= gc_records[i];
		struct s_Record		&record		= records[i];

		if(gc_record->m_result_ip->Length > 0)
			NNN::CLR::String_to_char(gc_record->m_result_ip, record.m_result_ip);

		if(gc_record->m_err_msg->Length > 0)
			NNN::CLR::String_to_char(gc_record->m_err_msg, record.m_err_msg);
	}	// for
}

}	// namespace ddns_server_CLR
