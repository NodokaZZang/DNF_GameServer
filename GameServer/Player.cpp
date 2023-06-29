#include "pch.h"
#include "Player.h"
#include <atomic>
Player::Player(Session* _session)
{
	static std::atomic<int32> playerSQ = 0;
	m_session = _session;
	playerSQ.fetch_add(1);
	m_playerSQ = playerSQ;
}

Player::~Player()
{

}
