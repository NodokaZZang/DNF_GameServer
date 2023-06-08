#pragma once
class Session;
class TCPListener
{
private:
	SOCKET			m_listenSocket;
	SOCKADDR_IN		m_serverAddr;
	Session*		(*m_sessionFactory)();
public:
	TCPListener(Session* (*sessionFactory)(void)) { m_sessionFactory = sessionFactory; };
	~TCPListener() {};

	void		Init();
	Session*	Accept();
	void		Clean();
};

