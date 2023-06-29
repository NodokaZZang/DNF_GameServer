#pragma once
class Session;
class Player 
{
private:
	int32 m_playerSQ = 0;
	int32 m_playerPick = 0; // 0 없음, 1 귀검사, 2거너
	Session* m_session = nullptr;
	int32 m_playerOrder = 0;
	bool m_ready = false;

public:
	Player(Session* _session);
	~Player();

	void SetPlayerOrder(int32 _playerOrder) { m_playerOrder = _playerOrder; }
	int32 GetPlayerSQ() { return m_playerSQ; }
	int32 GetPlayerPick() { return m_playerPick; }
	int32 GetPlayerOrder() { return m_playerOrder; }
	void SetPlayerPick(int32 _playerPick) { m_playerPick = _playerPick; }
	bool GetReady() { return m_ready; }
	void SetReady(bool _ready) { m_ready = _ready; }
};

