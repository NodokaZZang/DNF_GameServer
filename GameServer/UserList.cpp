#include "pch.h"
#include "UserList.h"
#include "GameSession.h"
#include "BufferReader.h"
#include "BufferWriter.h"

UserList* UserList::m_instance = nullptr;

UserList::UserList()
{
	InitializeCriticalSection(&m_userListLock);
}

UserList::~UserList()
{
	DeleteCriticalSection(&m_userListLock);
}

void UserList::Push(Session* _session)
{
	EnterCriticalSection(&m_userListLock);
	m_sessions.push_back(_session);
	LeaveCriticalSection(&m_userListLock);
}

void UserList::Pop(Session* _session)
{
	EnterCriticalSection(&m_userListLock);

	std::vector<Session*>::iterator deleteIt;
	for (deleteIt = m_sessions.begin(); deleteIt != m_sessions.end(); ++deleteIt)
		if (*deleteIt == _session)
			break;

	if (deleteIt != m_sessions.end())
		m_sessions.erase(deleteIt);

	LeaveCriticalSection(&m_userListLock);
}

void UserList::SendUserList(Session* _session, int32 _pageNum)
{
	int32 pageViewCount = 6;
	int32 pageCount = m_sessions.size() - 1;

	int32 firstIndex = pageCount - (_pageNum * pageViewCount);
	int32 lastIndex = max(firstIndex - (pageViewCount - 1),0);
	
	if (_pageNum > pageCount)
		return;

	if (_pageNum < 0)
		return;

	BYTE sendBuffer[4096];
	BufferWriter bw(sendBuffer);

	std::vector<Session*> userList;

	for (int32 index =  firstIndex; index >= lastIndex; index--)
	{
		userList.push_back(m_sessions[index]);
	}

	if (userList.size() == 0)
		return;


	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
	int32 userListSize = userList.size();
	bw.Write(userListSize);

	for (int32 i = 0; i < userList.size(); i++)
	{
		int32 strLen = wcslen(userList[i]->GetUsername()) * 2;
		bw.Write(strLen);
		bw.WriteWString(userList[i]->GetUsername(), strLen);
	}

	bw.Write(pageCount / pageViewCount);
	bw.Write(_pageNum);

	int32 usernameSize = wcslen(_session->GetUsername()) * 2;
	bw.Write(usernameSize);
	bw.WriteWString(_session->GetUsername(), usernameSize);


	pktHeader->_type = S_T_C_USERLIST;
	pktHeader->_pktSize = bw.GetWriterSize();
	_session->Send(sendBuffer, pktHeader->_pktSize);
}
