#pragma once
#include "Session.h"
class GameSession : public Session
{
public:
	static Session* MakeGameSession() { return new GameSession(); }
public:
	virtual void OnRecv(Session* session, BYTE* dataPtr, int32 dataLen);
	virtual void OnDisconnect();
	virtual void OnConnect();
};

