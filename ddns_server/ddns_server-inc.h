//--------------------------------------------------------------------------------------
// Copyright (c) AcgDev
// https://www.AcgDev.com/
//
// Desc : �����ļ�
//--------------------------------------------------------------------------------------

#pragma once
#ifndef _DDNS_SERVER___DDNS_SERVER_INC_H_
#define _DDNS_SERVER___DDNS_SERVER_INC_H_

#include "../nnn/Src/nnnSocket/nnnSocket.h"
#include "../common/Common-inc.h"

namespace DDNS_Server
{

// ����������״̬
enum struct es_State : BYTE
{
	Stopped,	// ֹͣ
	Running,	// ��������
	Exiting,	// ���ڹرճ���
};

}	// namespace DDNS_Server

#endif	// _DDNS_SERVER___DDNS_SERVER_INC_H_
