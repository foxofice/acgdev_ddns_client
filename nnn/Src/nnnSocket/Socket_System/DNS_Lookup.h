//--------------------------------------------------------------------------------------
// Copyright (c) AcgDev
// https://www.AcgDev.com/
//
// Desc : DNS Lookup
//--------------------------------------------------------------------------------------

#pragma once
#ifndef _NNNSOCKET___SOCKET_SYSTEM___DNS_LOOKUP_H_
#define _NNNSOCKET___SOCKET_SYSTEM___DNS_LOOKUP_H_

#include <string>
#include <map>

#include "nnnSocket-inc.h"

namespace NNN
{
namespace Socket
{

// DNS ����
NNN_API bool	DNS_Lookup(	const char									*dns_server_ip,	// DNS ������ IP
							const char									*domain_name,	// ����
							__out std::map<std::string, std::string>	&ip_list,		// ip -> domain_name
							__out int									&err_num,		// ������루����������� false��
							__out bool									&is_udp );		// �Ƿ� UDP ͨѶ�ɹ�

}	// namespace Socket
}	// namespace NNN

#endif	// _NNNSOCKET___SOCKET_SYSTEM___DNS_LOOKUP_H_
