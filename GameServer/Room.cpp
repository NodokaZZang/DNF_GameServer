#include "pch.h"
#include "Room.h"

Room::Room()
{
	static int32 roomSq = 0;
	m_roomSq = ++roomSq;
}

Room::~Room()
{
}

void Room::SetTitle(WCHAR* _title, int32 _strSize)
{
	::memcpy(m_title, _title, _strSize);
}

void Room::SetTitle(WCHAR* _title)
{
	wcscpy_s(m_title, _title);
}
