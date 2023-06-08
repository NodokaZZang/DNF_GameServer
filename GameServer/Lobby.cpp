#include "pch.h"
#include "Lobby.h"
#include "Session.h"
Lobby* Lobby::m_instance = nullptr;

Lobby::Lobby()
{
	InitializeCriticalSection(&m_lobbyLock);
}

Lobby::~Lobby()
{
	DeleteCriticalSection(&m_lobbyLock);
}

void Lobby::Push(Session* _session)
{
	EnterCriticalSection(&m_lobbyLock);
	m_sessions.insert(_session);
	LeaveCriticalSection(&m_lobbyLock);
}

void Lobby::Pop(Session* _session)
{
	EnterCriticalSection(&m_lobbyLock);
	m_sessions.erase(_session);
	LeaveCriticalSection(&m_lobbyLock);
}

void Lobby::BroadCast(BYTE* _buffer, int32 _dataSize)
{
	for (auto& session : m_sessions)
		session->Send(_buffer, _dataSize);
}
