#pragma once
#include "Session.h"
class GameSession : public Session
{
private:
	class Room* _room = nullptr;
	int32		_playerId = -1;

private:
	/* UPD Á¤º¸ */
	WCHAR m_udpServerIP[15] = {0};
	int32 m_port;

public:
	static Session* MakeGameSession() { return new GameSession(); }
public:
	virtual void OnRecv(Session* session, BYTE* dataPtr, int32 dataLen);
	virtual void OnDisconnect();
	virtual void OnConnect();
	void RoomPop();
	void SetPlayerId(int32 _id) { _playerId = _id; }
	void SetRoom(Room* room) { _room = room; }
	int32 GetPlayerID() { return _playerId; }
	Room* GetRoom() { return _room; }
	void SetUDPInfo(WCHAR* ip,  int32 ipSize, int32 port);
	WCHAR* GetUDPIP() { return m_udpServerIP; }
	int32 GetUDPPort() { return m_port; }
};

