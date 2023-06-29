#pragma once
class Player;
class Session;
class Room
{
private:
	std::atomic<bool> m_initEnd = false;
	int32 m_roomSq = 0;
	WCHAR m_title[30] = {0};
	Player* m_player[4] = { nullptr, nullptr, nullptr, nullptr };
	class GameSession* m_sessions[4] = { nullptr, nullptr, nullptr, nullptr };
	int32 m_adminPlayerSQ = 0;
	int32 m_playerOrder = 0;
	CRITICAL_SECTION	m_roomLock;
	int32				m_status = 0; // 0 대기 , 1 게임
	std::atomic<int32>	m_joinCnt = 0;
	class GameStage* m_gameStage = nullptr;
public:	
	Room();
	~Room();

	void SetTitle(WCHAR* _title, int32 _strSize);
	void SetTitle(WCHAR* _title);
	void SetRoomSq(int32 _roomSq);
	int32 GetRoomSQ() { return m_roomSq; }
	WCHAR* GetTitle() { return m_title; }

	void AddPlayerAdmin(Session* _session, Player* _player);
	void AddPlayer(Session* _session, Player* _player);
	Player* GetPlayer(int32 _playerId);
	bool RemovePlayer(int32 _playerId);

	int32 GetPlayerOrder();
	int32 GetStatus() {return m_status;}
	int32 GetJoinCnt() { return m_joinCnt; }

	void JoinUser(Session* _session);
	void OutUser(Session* _session);
	void BroadCast(BYTE* _dataPtr, int32 dataSize);
	void Sync();
	void UserPick(Session* _session, int32 userPick);
	void UserReady(Session* _session, bool ready, bool admin);

	void GameStart();
	int32 GetAdminPlayerID() { return m_adminPlayerSQ; }
	void GameInit(Session* _session);
	void SessionRoomDelete();
};

