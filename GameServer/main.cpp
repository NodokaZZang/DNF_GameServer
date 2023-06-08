#include "pch.h"
#include "IOCPCore.h"
#include "Session.h"
#include "TCPListener.h"
#include "Service.h"
#include "GameSession.h"
#include "Lobby.h"
#include "UserList.h"
#include "RoomList.h"
#include "Room.h"

int main() 
{
	setlocale(LC_ALL, "korean");
	_wsetlocale(LC_ALL, L"korean");

	IOCPCore iocp;
	Session* (*sessionFactory)(void) = GameSession::MakeGameSession;
	TCPListener tcpListener(sessionFactory);

	for (int i = 0; i < 200; i++)
	{
		GameSession* dummy = new GameSession();
		WCHAR buffer[256] = { 0 };
		swprintf_s(buffer, _countof(buffer), L"dummyClient%d", i + 1);
		dummy->SetUsername(buffer);
		UserList::GetInstance()->Push(dummy);
	}


	Service service(&iocp, &tcpListener);
	return 0;
}