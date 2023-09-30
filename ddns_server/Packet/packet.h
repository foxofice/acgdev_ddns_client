//--------------------------------------------------------------------------------------
// Copyright (c) AcgDev
// https://www.AcgDev.com/
//
// Desc : ���ݰ����շ���
//--------------------------------------------------------------------------------------

#pragma once
#ifndef _DDNS_SERVER___PACKET___PACKET_H_
#define _DDNS_SERVER___PACKET___PACKET_H_

#include "../../nnn/Src/nnnSocket/nnnSocket.h"
#include "../../ddns_server_CLR/ddns_server_CLR.h"

#include "../../common/Common-inc.h"

namespace DDNS_Server
{
namespace Packet
{

//================================================================================
// Send
//================================================================================

#pragma region Send
// Server ����������
// S xxxx
void	send_KeepAlive(struct NNN::Socket::s_SessionData *sd);

// Server ��Ӧ Ping
/*
	S xxxx <client_time>.double

<client_time>	- �ͻ��˷��͵� NNN::Time::get_time()
*/
void	send_Ping(struct NNN::Socket::s_SessionData *sd, double client_time);

// ����¼��֤��

// Server ���� KeyIV
/*
	S xxxx aes({<send_Key>.32B <send_IV>.16B})

<send_Key>/<send_IV>	- ������ɵ� Key/IV
aes �� Key/IV			- Common::Encrypt::gen_KeyIV(user, pwd, ...)
*/
void	send_KeyIV(struct NNN::Socket::s_SessionData *sd);

// Server ���͡���¼�����
/*
	S xxxx <result>.B

<result>	- OK��Failed
*/
void	send_Login_Result(struct NNN::Socket::s_SessionData *sd, es_Result result);

//����¼��֤��

// Server ���͡����������� A/AAAA ��¼�Ľ����
/*
	S xxxx <packet_len>.USHORT <aes_data>.?B

	<aes_data> =
	{
		<ip_len>.B <ip>.?B(char*)

		<count>.USHORT
		{
			<name_len>.B <name>.?B(char*)
			<domain_len>.B <domain>.?B(char*)
			<user_idx>.int
			{<ok>.b <failed>.b xxxxxx}.B
			<err_msg_len>.B <err_msg>.?B(WCHAR*)
		}*
	}
*/
void	send_Update_Domains_Result(	struct NNN::Socket::s_SessionData					*sd,
									const char											ip[46],
									const std::vector<struct ddns_server_CLR::s_Record>	&records );
#pragma endregion

//================================================================================
// Recv
//================================================================================

#pragma region Recv
// ���� packet
void	parse_packet(struct NNN::Socket::s_SessionData *sd);

#define	PARSE_FUNC(name)	es_Parse_Result name(struct NNN::Socket::s_SessionData *sd)

// Client ���� Ping
/*
	R xxxx <client_time>.double

<client_time>	- �ͻ��˷��͵� NNN::Time::get_time()
*/
PARSE_FUNC(recv_Ping);

// ����¼��֤��

// Client ���͡���¼���ݡ�
/*
	R xxxx aes({<send_Key>.32B <send_IV>.16B})

<send_Key>/<send_IV>	- Server ֮ǰ�Զ����ɵ� Key/IV
aes �� Key/IV			- Common::Encrypt::gen_KeyIV(pwd, user, ...)
*/
PARSE_FUNC(recv_Login_Data);

//����¼��֤��

// Client ���͡����������� A/AAAA ��¼��
/*
	R xxxx <packet_len>.USHORT <aes_data>.?B

	<aes_data> =
	{
		<Key_len>.B <Key>.?B(char*)
		<Secret_len>.B <Secret>.?B(char*)

		<ip_len>.B [<ip>.?B(char*)]

		<domains_count>.USHORT
		{
			<name_len>.B <name>.?B(char*)
			<domain_len>.B <domain>.?B(char*)
			<TTL>.int
			<user_idx>.int
		}*
	}

��� <ip_len> = 0����ʹ�� Server �������ӵ� IP
*/
PARSE_FUNC(recv_Update_Domains);

#undef PARSE_FUNC
#pragma endregion

}	// namespace Packet
}	// namespace DDNS_Server

#endif	// _DDNS_SERVER___PACKET___PACKET_H_
