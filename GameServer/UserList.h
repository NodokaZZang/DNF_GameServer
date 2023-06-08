#pragma once
class Session;
class UserList
{
private:
	static UserList*	  m_instance;
	std::vector<Session*> m_sessions;
	CRITICAL_SECTION	  m_userListLock;
public:
	UserList();
	~UserList();

	static UserList* GetInstance() { if (m_instance == nullptr) m_instance = new UserList(); return m_instance; }
	void Push(Session* _session);
	void Pop(Session* _session);
	void SendUserList(Session* _session, int32 _pageNum);
};

