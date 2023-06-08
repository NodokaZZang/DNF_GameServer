#include "pch.h"
#include "PacketHandler.h"
#include "GameSession.h"
#include "Lobby.h"
#include "UserList.h"
#include "BufferReader.h"
#include "RoomList.h"
#include "BufferWriter.h"
void PacketHandler::PakcetHandle(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	PacketHeader* header =reinterpret_cast<PacketHeader*>(_dataPtr);
	BYTE* dataPtr = _dataPtr + sizeof(PacketHeader);
	int32 dataSize = _dataSize - sizeof(PacketHeader);

	switch (header->_type)
	{
	case C_T_S_LOGIN:
		PakcetHandle_C_T_S_LOGIN(_session,dataPtr, dataSize);
		break;

	case C_T_S_USERLIST:
		PakcetHandle_C_T_S_USERLIST(_session, dataPtr, dataSize);
		break;

	case C_T_S_ROOMLIST:
		PakcetHandleC_T_S_ROOMLIST(_session, dataPtr, dataSize);
		break;

	case C_T_S_CHATSEND_LOBBY:
		PakcetHandleC_T_S_CHATSEND(_session, dataPtr, dataSize);
		break;
	}
}

void PacketHandler::PakcetHandle_C_T_S_LOGIN(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	WCHAR* username = (WCHAR*)(_dataPtr);
	_session->SetUsername((WCHAR*)_dataPtr);
	wprintf(L"User Login : %s \n", _session->GetUsername());
	UserList::GetInstance()->Push(_session);
	Lobby::GetInstance()->Push(_session);

	BYTE sendBuffer[4096];
	BufferWriter bw(sendBuffer);
	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
	
	pktHeader->_type = S_T_C_LOGIN;
	pktHeader->_pktSize = bw.GetWriterSize();
	
	_session->Send(sendBuffer, pktHeader->_pktSize);
}

void PacketHandler::PakcetHandle_C_T_S_USERLIST(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	int32 requestPageNum;
	br.Read(requestPageNum);
	UserList::GetInstance()->SendUserList(_session,requestPageNum);
}

void PacketHandler::PakcetHandleC_T_S_ROOMLIST(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	int32 requestPageNum;
	br.Read(requestPageNum);
	RoomList::GetInstance()->SendRoomList(_session, requestPageNum);
}

void PacketHandler::PakcetHandleC_T_S_CHATSEND(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	WCHAR wText[256] = {0};
	br.ReadWString(wText, _dataSize);
	WCHAR buffer[1024] = { 0 };
	swprintf_s(buffer, _countof(buffer), L"%s : %s", _session->GetUsername(), wText);

	BYTE sendBuffer[4096];
	BufferWriter bw(sendBuffer);
	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
	int32 strSize = wcslen(buffer) * 2;
	bw.Write(strSize);
	bw.WriteWString(buffer, strSize);

	pktHeader->_type = S_T_C_CHATSEND_LOBBY;
	pktHeader->_pktSize = bw.GetWriterSize();
	Lobby::GetInstance()->BroadCast(sendBuffer, pktHeader->_pktSize);
}
