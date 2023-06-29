#pragma once
#define RoomMaxCount 3000
class Room;
class Session;
class RoomList
{
private:
	static RoomList*	m_instance;
	std::vector<Room*>	m_rooms;
	CRITICAL_SECTION	m_roomListLock;
	int32				m_roomOrder = 0;

public:
	RoomList();
	~RoomList();

	static RoomList* GetInstance() { if (m_instance == nullptr) m_instance = new RoomList(); return m_instance; }
	void Push(Room* _session);
	void Pop(int32 _roomSq);
	void SendRoomList(Session* _session, int32 _pageNum);
	Room* GetRoom(int32 _roomSQ);
};

