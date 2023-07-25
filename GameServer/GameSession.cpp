#include "pch.h"
#include "GameSession.h"
#include "PacketHandler.h"
#include "Lobby.h"
#include "NetAddress.h"
#include "UserList.h"
#include "Room.h"
void GameSession::OnRecv(Session* session, BYTE* dataPtr, int32 dataLen)
{
	PacketHandler::PakcetHandle(session,dataPtr, dataLen);
}

void GameSession::OnDisconnect()
{
	wprintf(L"GameSession OnDisconnect %s \n", m_netAddress->GetClientIPSTR());
	Lobby::GetInstance()->Pop(this);
	UserList::GetInstance()->Pop(this);
	RoomPop();
}

void GameSession::OnConnect()
{
	wprintf(L"GameSession Onconnect %s \n", m_netAddress->GetClientIPSTR());
}

void GameSession::RoomPop()
{
	if (_room != nullptr) 
	{
		_room->RemovePlayer(_playerId);
	}
	_room = nullptr;
}

void GameSession::SetUDPInfo(WCHAR* ip, int32 ipSize, WCHAR* localip, int32 localipSize, int32 port)
{
	::memcpy(m_udpServerIP, ip, ipSize);
	::memcpy(m_udpServerLocalIP, localip, localipSize);
	m_port = port;
}
