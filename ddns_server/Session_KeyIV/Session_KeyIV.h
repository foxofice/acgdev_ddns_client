//--------------------------------------------------------------------------------------
// Copyright (c) AcgDev
// https://www.AcgDev.com/
//
// Desc : ���� session �� KeyIV
//--------------------------------------------------------------------------------------

#pragma once
#ifndef _DDNS_SERVER___SESSION_KEYIV___SESSION_KEYIV_H_
#define _DDNS_SERVER___SESSION_KEYIV___SESSION_KEYIV_H_

#include "../../common/s_AES_KeyIV.h"

namespace DDNS_Server
{
namespace Session_KeyIV
{

// ��ʼ��/����
HRESULT				DoInit();
HRESULT				DoFinal();

// �����µ� s_AES_KeyIV������ session ��������� session_id ��Ӧ�� s_AES_KeyIV �Ѵ��ڣ��򷵻� nullptr��
struct s_AES_KeyIV*	add_KeyIV(UINT64 session_id);

// �Ƴ� s_AES_KeyIV
void				remove_KeyIV(UINT64 session_id);

}	// namespace Session_KeyIV
}	// namespace DDNS_Server

#endif	// _DDNS_SERVER___SESSION_KEYIV___SESSION_KEYIV_H_