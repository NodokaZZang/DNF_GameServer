#include "pch.h"
#include "Player.h"
#include "GameSession.h"
#include "Room.h"
#include "GameSession.h"
#include "RoomList.h"
#include "BufferWriter.h"
#include "Lobby.h"
#include "BufferReader.h"
#include "NetAddress.h"
Room::Room()
{
	InitializeCriticalSection(&m_roomLock);
}

Room::~Room()
{
	DeleteCriticalSection(&m_roomLock);
}

void Room::SetTitle(WCHAR* _title, int32 _strSize)
{
	::memcpy(m_title, _title, _strSize);
}

void Room::SetTitle(WCHAR* _title)
{
	wcscpy_s(m_title, _title);
}

void Room::SetRoomSq(int32 _roomSq)
{
	m_roomSq = _roomSq;
}

void Room::AddPlayerAdmin(Session* _session, Player* _player)
{
	EnterCriticalSection(&m_roomLock);

	int32 playerId = _player->GetPlayerSQ();
	int32 playerOrder = GetPlayerOrder();
	
	m_player[playerOrder] = _player;
	m_sessions[playerOrder] = static_cast<GameSession*>(_session);
	_player->SetPlayerOrder(playerOrder);
	m_adminPlayerSQ = playerId;

	m_joinCnt.fetch_add(1);
	static_cast<GameSession*>(_session)->SetPlayerId(playerId);
	static_cast<GameSession*>(_session)->SetRoom(this);
	LeaveCriticalSection(&m_roomLock);
}

void Room::AddPlayer(Session* _session, Player* _player)
{
	EnterCriticalSection(&m_roomLock);
	int32 playerId = _player->GetPlayerSQ();
	int32 playerOrder = GetPlayerOrder();

	m_player[playerOrder] = _player;
	m_sessions[playerOrder] = static_cast<GameSession*>(_session);
	_player->SetPlayerOrder(playerOrder);

	m_joinCnt.fetch_add(1);
	static_cast<GameSession*>(_session)->SetPlayerId(playerId);
	static_cast<GameSession*>(_session)->SetRoom(this);
	LeaveCriticalSection(&m_roomLock);
}

Player* Room::GetPlayer(int32 _playerId)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_player[i] != nullptr) 
		{
			if (m_player[i]->GetPlayerSQ() == _playerId) 
			{
				return m_player[i];
			}
		}	
	} 
	return nullptr;
}

bool Room::RemovePlayer(int32 _playerId)
{
	EnterCriticalSection(&m_roomLock);
	if (GetPlayer(_playerId) != nullptr) 
	{
		if (_playerId == m_adminPlayerSQ) 
		{
			for (int i = 0; i < 4; i++)
			{
				Player* pp = m_player[i];
				if (pp != nullptr) 
				{
					delete m_player[i];
					m_player[i] = nullptr;

					BYTE sendBuffer[1024];
					BufferWriter bw(sendBuffer);
					Lobby::GetInstance()->Push(m_sessions[i]);
					PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
					pktHeader->_type = S_T_C_WATINGROOMOUTUSER;
					pktHeader->_pktSize = bw.GetWriterSize();
					m_sessions[i]->Send(sendBuffer, pktHeader->_pktSize);
					m_sessions[i]->SetRoom(nullptr);
					m_sessions[i]->SetPlayerId(0);
					m_sessions[i] = nullptr;
				}
			}

			RoomList::GetInstance()->Pop(m_roomSq);
			LeaveCriticalSection(&m_roomLock);
			return true;
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				Player* p = m_player[i];
				if (p != nullptr && p->GetPlayerSQ() == _playerId)
				{
					delete m_player[i];
					m_player[i] = nullptr;
					m_sessions[i]->SetRoom(nullptr);
					m_sessions[i]->SetPlayerId(0);
					m_sessions[i] = nullptr;
					m_joinCnt.fetch_sub(1);
					LeaveCriticalSection(&m_roomLock);
					Sync();
					break;
				}
			}

			if (m_joinCnt == 0)
			{
				RoomList::GetInstance()->Pop(m_roomSq);
				LeaveCriticalSection(&m_roomLock);
				return true;
			}
		}
		return false;
	}
	
}

int32 Room::GetPlayerOrder()
{
	for (int i = 0; i < 4; i++) 
	{
		if (m_player[i] == nullptr)
			return i;
	}
}

void Room::JoinUser(Session* _session)
{
	Sync();
}

void Room::OutUser(Session* _session)
{
	BYTE sendBuffer[1024];
	BufferWriter bw(sendBuffer);

	bool roomEnd = RemovePlayer(static_cast<GameSession*>(_session)->GetPlayerID());

	Lobby::GetInstance()->Push(_session);

	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
	pktHeader->_type = S_T_C_WATINGROOMOUTUSER;
	pktHeader->_pktSize = bw.GetWriterSize();
	_session->Send(sendBuffer, pktHeader->_pktSize);
}

void Room::BroadCast(BYTE* _dataPtr, int32 dataSize)
{
	for (int i = 0; i < 4; i++) 
	{
		if (m_sessions[i] != nullptr) 
		{
			m_sessions[i]->Send(_dataPtr, dataSize);
		}
	}
}

void Room::Sync()
{
	EnterCriticalSection(&m_roomLock);
	for (int i = 0; i < 4; i++)
	{
		Player* player = m_player[i];
		Session* session = m_sessions[i];

		if (player != nullptr)
		{
			int32 playerId = player->GetPlayerSQ();
			int32 playerOrder = m_player[i]->GetPlayerOrder();
			BYTE sendBuffer[4096];
			BufferWriter bw(sendBuffer);
			PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
			bw.Write(playerId);
			bw.Write(playerOrder);
			bw.Write(m_joinCnt.load());
			for (int j = 0; j < 4; j++)
			{
				if (m_player[j] != nullptr)
				{
					bw.Write(j);
					int32 strLen = wcslen(m_sessions[j]->GetUsername()) * 2;
					bw.Write(strLen);
					bw.WriteWString(m_sessions[j]->GetUsername(), strLen);
					bw.Write(m_player[j]->GetPlayerPick());
					bw.Write(m_player[j]->GetReady());
				}
			}
			pktHeader->_pktSize = bw.GetWriterSize();
			pktHeader->_type = S_T_C_WATINGROOMINIT;
			session->Send(sendBuffer, pktHeader->_pktSize);
		}
	}

	Player* admin = GetPlayer(m_adminPlayerSQ);
	LeaveCriticalSection(&m_roomLock);

	if (admin != nullptr)
	{
		int32 order = admin->GetPlayerOrder();
		BYTE sendBuffer[4096];
		BufferWriter bw(sendBuffer);
		PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
		bw.Write(m_adminPlayerSQ);
		pktHeader->_pktSize = bw.GetWriterSize();
		pktHeader->_type = S_T_C_WATINGROOMADMIN;
		m_sessions[order]->Send(sendBuffer, pktHeader->_pktSize);
	}
}

void Room::UserPick(Session* _session, int32 userPick)
{
	EnterCriticalSection(&m_roomLock);
	GameSession* gs = static_cast<GameSession*>(_session);
	Player* p = GetPlayer(gs->GetPlayerID());
	if (p != nullptr) 
	{
		p->SetPlayerPick(userPick);
		Sync();
	}
	LeaveCriticalSection(&m_roomLock);
}

void Room::UserReady(Session* _session, bool ready, bool admin)
{
	EnterCriticalSection(&m_roomLock);
	if (admin) 
	{
		for (int i = 0; i < 4; i++) 
		{
			if (m_player[i] != nullptr) 
			{
				if (m_player[i]->GetPlayerPick() == 0)
				{
					LeaveCriticalSection(&m_roomLock);
					return;
				}

				if (m_player[i]->GetPlayerSQ() == m_adminPlayerSQ)
					continue;
				
				else if (m_player[i]->GetReady() == false)
				{
					LeaveCriticalSection(&m_roomLock);
					return;
				}
			}
		}
		
		{
			GameStart();
		}
	}
	else 
	{
		GameSession* gs = static_cast<GameSession*>(_session);
		Player* p = GetPlayer(gs->GetPlayerID());
		if (p != nullptr)
		{
			p->SetReady(ready);
			Sync();
		}
	}
	LeaveCriticalSection(&m_roomLock);
}

void Room::GameStart()
{
	BYTE sendBuffer[1024];
	BufferWriter bw(sendBuffer);
	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
	pktHeader->_pktSize = bw.GetWriterSize();
	pktHeader->_type = S_T_C_GAMESTART;
	m_status = 1;

	BroadCast(sendBuffer, pktHeader->_pktSize);
}

void Room::GameInit(Session* _session)
{
	GameSession* gameSession = static_cast<GameSession*>(_session);

	BYTE sendBuffer[4096];
	BufferWriter bw(sendBuffer);
	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();

	int32 ownerPlayerId = gameSession->GetPlayerID();
	bw.Write(ownerPlayerId);
	bw.Write(m_adminPlayerSQ);
	bw.Write(m_joinCnt.load());

	for (int i = 0; i < 4; i++) 
	{
		if (m_player[i] == nullptr)
			continue;

		GameSession* gs = static_cast<GameSession*>(m_sessions[i]);

		int32 playerId = m_player[i]->GetPlayerSQ();
		int32 playerPick = m_player[i]->GetPlayerPick();

		bw.Write(playerId);
		bw.Write(playerPick);

		int32 strSize = wcslen(gs->GetUDPIP()) * 2;
		
		bw.Write(strSize);
		bw.WriteWString(gs->GetUDPIP(), strSize);

		int32 localstrSize = wcslen(gs->GetUDPLocalIP()) * 2;

		bw.Write(localstrSize);
		bw.WriteWString(gs->GetUDPLocalIP(), localstrSize);

		bw.Write(gs->GetUDPPort());
		
		int32 usernameSize = wcslen(gs->GetUsername()) * 2;
		
		bw.Write(usernameSize);
		bw.WriteWString(gs->GetUsername(), usernameSize);
	}

	pktHeader->_pktSize = bw.GetWriterSize();
	pktHeader->_type = S_T_C_GAMEINIT;
	_session->Send(sendBuffer, pktHeader->_pktSize);
}

void Room::SessionRoomDelete()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_sessions[i])
		{
			static_cast<GameSession*>(m_sessions[i])->SetRoom(nullptr);
			Lobby::GetInstance()->Push(m_sessions[i]);
		}
	}
}
