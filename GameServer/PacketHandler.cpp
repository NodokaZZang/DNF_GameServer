#include "pch.h"
#include "PacketHandler.h"
#include "GameSession.h"
#include "Lobby.h"
#include "UserList.h"
#include "BufferReader.h"
#include "RoomList.h"
#include "BufferWriter.h"
#include "Room.h"
#include "Player.h"
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
		PakcetHandle_C_T_S_ROOMLIST(_session, dataPtr, dataSize);
		break;

	case C_T_S_CHATSEND_LOBBY:
		PakcetHandle_C_T_S_CHATSEND(_session, dataPtr, dataSize);
		break;

	case C_T_S_CREATEROOM:
		PakcetHandle_C_T_S_CREATEROOM(_session, dataPtr, dataSize);
		break;

	case C_T_S_ENTEROOM:
		PakcetHandle_C_T_S_ENTEROOM(_session, dataPtr, dataSize);
		break;

	case C_T_S_WATINGROOMINIT:
		PacketHandle_C_T_S_WATINGROOMINIT(_session, dataPtr, dataSize);
		break;

	case C_T_S_WATINGROOMOUTUSER:
		PacketHandle_C_T_S_WATINGROOMOUTUSER(_session, dataPtr, dataSize);
		break;

	case C_T_S_CHATSEND_WATINGROOM:
		PacketHandle_C_T_S_CHATSEND_WATINGROOM(_session, dataPtr, dataSize);
		break;

	case C_T_S_USERPICK:
		PacketHandle_C_T_S_USERPICK(_session, dataPtr, dataSize);
		break;

	case C_T_S_USERREADY:
		PacketHandle_C_T_S_USERREADY(_session, dataPtr, dataSize);
		break;

	case C_T_S_GAMEINIT:
		PacketHandle_C_T_S_GAMEINIT(_session, dataPtr, dataSize);
		break;

	case C_T_S_UDPIPPORTSEND:
		PacketHandle_C_T_S_UDPIPPORTSEND(_session, dataPtr, dataSize);
		break;

	case C_T_S_GAMEEND:
		PacketHandle_C_T_S_GAMEEND(_session, dataPtr, dataSize);
		break;
	}
}

void PacketHandler::PakcetHandle_C_T_S_LOGIN(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);

	WCHAR username[256] = { 0 };
	br.ReadWString(username, _dataSize);

	_session->SetUsername(username);

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

void PacketHandler::PakcetHandle_C_T_S_ROOMLIST(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	int32 requestPageNum;
	br.Read(requestPageNum);
	RoomList::GetInstance()->SendRoomList(_session, requestPageNum);
}

void PacketHandler::PakcetHandle_C_T_S_CHATSEND(Session* _session, BYTE* _dataPtr, int _dataSize)
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

void PacketHandler::PakcetHandle_C_T_S_CREATEROOM(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	int32 textLen;
	br.Read(textLen);

	WCHAR wText[256] = { 0 };
	br.ReadWString(wText, textLen);

	Room* newRoom = new Room();
	newRoom->SetTitle(wText);

	Player* player = new Player(_session);
	newRoom->AddPlayerAdmin(_session,player);

	RoomList::GetInstance()->Push(newRoom);

	BYTE sendBuffer[4096];
	BufferWriter bw(sendBuffer);
	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();

	pktHeader->_type = S_T_C_ENTEROOM;

	bw.Write(newRoom->GetRoomSQ());

	pktHeader->_pktSize = bw.GetWriterSize();

	Lobby::GetInstance()->Pop(_session);
	_session->Send(sendBuffer, pktHeader->_pktSize);

}

void PacketHandler::PakcetHandle_C_T_S_ENTEROOM(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	int32 roomSQ;
	br.Read(roomSQ);

	Room* room = RoomList::GetInstance()->GetRoom(roomSQ);
	
	if (room == nullptr) return;

	if (room->GetStatus() == 1) 
	{
		return;
	}

	if (room->GetJoinCnt() == 4) 
	{	
		return;
	}

	Player* player = new Player(_session);
	room->AddPlayer(_session, player);

	Lobby::GetInstance()->Pop(_session);


	BYTE sendBuffer[4096];
	BufferWriter bw(sendBuffer);
	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();

	pktHeader->_type = S_T_C_ENTEROOM;

	bw.Write(room->GetRoomSQ());

	pktHeader->_pktSize = bw.GetWriterSize();

	_session->Send(sendBuffer, pktHeader->_pktSize);
}

void PacketHandler::PacketHandle_C_T_S_WATINGROOMINIT(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	int32 roomSQ;
	br.Read(roomSQ);
	RoomList::GetInstance()->GetRoom(roomSQ)->JoinUser(_session);
}

void PacketHandler::PacketHandle_C_T_S_WATINGROOMOUTUSER(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	int32 playerOrder;
	br.Read(playerOrder);
	static_cast<GameSession*>(_session)->GetRoom()->OutUser(_session);
}

void PacketHandler::PacketHandle_C_T_S_CHATSEND_WATINGROOM(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	WCHAR wText[256] = { 0 };
	br.ReadWString(wText, _dataSize);
	WCHAR buffer[1024] = { 0 };
	swprintf_s(buffer, _countof(buffer), L"%s : %s", _session->GetUsername(), wText);

	BYTE sendBuffer[4096];
	BufferWriter bw(sendBuffer);
	PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
	int32 strSize = wcslen(buffer) * 2;
	bw.Write(strSize);
	bw.WriteWString(buffer, strSize);

	pktHeader->_type = S_T_S_CHATSEND_WATINGROOM;
	pktHeader->_pktSize = bw.GetWriterSize();
	static_cast<GameSession*>(_session)->GetRoom()->BroadCast(sendBuffer, pktHeader->_pktSize);
}

void PacketHandler::PacketHandle_C_T_S_USERPICK(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	int32 userPick;
	br.Read(userPick);

	static_cast<GameSession*>(_session)->GetRoom()->UserPick(_session, userPick);
}

void PacketHandler::PacketHandle_C_T_S_USERREADY(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	bool ready;
	bool admin;
	br.Read(ready);
	br.Read(admin);

	static_cast<GameSession*>(_session)->GetRoom()->UserReady(_session, ready, admin);
}

void PacketHandler::PacketHandle_C_T_S_GAMEINIT(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	static_cast<GameSession*>(_session)->GetRoom()->GameInit(_session);
}

void PacketHandler::PacketHandle_C_T_S_UDPIPPORTSEND(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	BufferReader br(_dataPtr);
	int32 ipSize;
	int32 localipSize;
	int32 port;

	WCHAR ip[20] = { 0 };
	WCHAR localIp[20] = { 0 };

	br.Read(ipSize);
	br.ReadWString(ip, ipSize);
	br.Read(localipSize);
	br.ReadWString(localIp, localipSize);
	br.Read(port);

	static_cast<GameSession*>(_session)->SetUDPInfo(ip, ipSize, localIp, localipSize, port);
}

//void PacketHandler::PacketHandle_C2S_GAMEEND(Session* _session, BYTE* _dataPtr, int _dataSize)

void PacketHandler::PacketHandle_C_T_S_GAMEEND(Session* _session, BYTE* _dataPtr, int _dataSize)
{
	int32 roomSQ = static_cast<GameSession*>(_session)->GetRoom()->GetRoomSQ();
	RoomList::GetInstance()->Pop(roomSQ);
	static_cast<GameSession*>(_session)->SetRoom(nullptr);
}
