#pragma once
class Session;
class Lobby
{
private:
	static Lobby*		  m_instance;
	std::set<Session*>	  m_sessions;
	CRITICAL_SECTION	  m_lobbyLock;
public:
	Lobby();
	~Lobby();

	static Lobby* GetInstance() { if (m_instance == nullptr) m_instance = new Lobby(); return m_instance; }
	void Push(Session* _session);
	void Pop(Session* _session);
	void BroadCast(BYTE* _buffer, int32 _dataSize);
};

