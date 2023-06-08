#pragma once
class Room;
class Session;
class RoomList
{
private:
	static RoomList*	m_instance;
	std::vector<Room*>	m_rooms;
	CRITICAL_SECTION	m_roomListLock;
public:
	RoomList();
	~RoomList();

	static RoomList* GetInstance() { if (m_instance == nullptr) m_instance = new RoomList(); return m_instance; }
	void Push(Room* _session);
	void Pop(Room* _session);
	void SendRoomList(Session* _session, int32 _pageNum);
};

