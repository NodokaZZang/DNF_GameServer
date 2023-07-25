#include "pch.h"
#include "RoomList.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "Room.h"

RoomList* RoomList::m_instance = nullptr;

RoomList::RoomList()
{
	InitializeCriticalSection(&m_roomListLock);
}

RoomList::~RoomList()
{
	DeleteCriticalSection(&m_roomListLock);
}

void RoomList::Push(Room* _room)
{
	EnterCriticalSection(&m_roomListLock);
	static int32 _roomSQ = 0;
	_room->SetRoomSq(++_roomSQ);

	m_rooms.push_back(_room);
	LeaveCriticalSection(&m_roomListLock);
}

void RoomList::Pop(int32 _roomSQ)
{
	EnterCriticalSection(&m_roomListLock);

	for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
	{
		if ((*it)->GetRoomSQ() == _roomSQ)
		{
			(*it)->SessionRoomDelete();
			delete (*it);
			m_rooms.erase(it);
			break;
		}
	}

	LeaveCriticalSection(&m_roomListLock);
}

void RoomList::SendRoomList(Session* _session, int32 _pageNum)
{
	int32 pageViewCount = 6;
	int32 pageCount = m_rooms.size() - 1;

	int32 firstIndex = pageCount - (_pageNum * pageViewCount);
	int32 lastIndex = max(firstIndex - (pageViewCount - 1), 0);
	
	BYTE sendBuffer[4096];
	BufferWriter bw(sendBuffer);
	std::vector<Room*> roomList;

	if (pageCount == -1) 
	{
		PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
		int32 roomListSize = roomList.size();
		bw.Write(roomListSize);
		bw.Write(0);
		bw.Write(0);

		pktHeader->_type = S_T_C_ROOMLIST;
		pktHeader->_pktSize = bw.GetWriterSize();
		_session->Send(sendBuffer, pktHeader->_pktSize);
	}
	else
	{
		if (_pageNum > pageCount)
			return;

		if (_pageNum < 0)
			return;

		for (int32 index = firstIndex; index >= lastIndex; index--)
		{
			roomList.push_back(m_rooms[index]);
		}

		if (roomList.size() == 0)
			return;

		PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
		int32 roomListSize = roomList.size();
		bw.Write(roomListSize);

		for (int32 i = 0; i < roomList.size(); i++)
		{
			bw.Write(roomList[i]->GetRoomSQ());
			bw.Write(firstIndex + i + 1);
			bw.Write(roomList[i]->GetStatus());

			int32 strLen = wcslen(roomList[i]->GetTitle()) * 2;
			bw.Write(strLen);
			bw.WriteWString(roomList[i]->GetTitle(), strLen);
			bw.Write(roomList[i]->GetJoinCnt());
		}

		bw.Write(pageCount / pageViewCount);
		bw.Write(_pageNum);

		pktHeader->_type = S_T_C_ROOMLIST;
		pktHeader->_pktSize = bw.GetWriterSize();
		_session->Send(sendBuffer, pktHeader->_pktSize);
	}
}

Room* RoomList::GetRoom(int32 _roomSQ)
{
	for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
	{
		if ((*it)->GetRoomSQ() == _roomSQ)
		{
			return *it;
		}
	}

	return nullptr;
}
