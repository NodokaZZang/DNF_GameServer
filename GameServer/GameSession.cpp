#include "pch.h"
#include "GameSession.h"
#include "PacketHandler.h"
#include "Lobby.h"
#include "NetAddress.h"
#include "UserList.h"
void GameSession::OnRecv(Session* session, BYTE* dataPtr, int32 dataLen)
{
	wprintf(L"GameSession OnRecv %s : recvSize : %d \n", m_netAddress->GetClientIPSTR(), dataLen);
	PacketHandler::PakcetHandle(session,dataPtr, dataLen);
}

void GameSession::OnDisconnect()
{
	wprintf(L"GameSession OnDisconnect %s \n", m_netAddress->GetClientIPSTR());
	Lobby::GetInstance()->Pop(this);
	UserList::GetInstance()->Pop(this);
}

void GameSession::OnConnect()
{
	wprintf(L"GameSession Onconnect %s \n", m_netAddress->GetClientIPSTR());
}
