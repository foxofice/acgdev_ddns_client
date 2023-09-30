//--------------------------------------------------------------------------------------
// Copyright (c) AcgDev
// https://www.AcgDev.com/
//
// Desc : ddns_server ִ�е� .Net ����
//--------------------------------------------------------------------------------------
#pragma once

#include "../nnn/Src/common/common-macro.h"

namespace ddns_server_CLR
{

struct s_Record
{
	char	m_name[50]		= {};
	char	m_domain[100]	= {};
	int		m_TTL			= 0;
	int		m_user_idx		= 0;		// �������û��Զ������ݣ�

	char	m_result_ip[46]		= {};	// ���³ɹ���� IP
	char	m_err_msg[0xff + 1]	= {};	// ������Ϣ
};

// ��ʼ��
NNN_API void	DoInit();

// ִ�и��� IP ��¼
NNN_API void	update_records_step3_update_by_server(	const char								*ip,
														const char								*Key,
														const char								*Secret,
														__inout std::vector<struct s_Record>	&records );

}	// namespace ddns_server_CLR
