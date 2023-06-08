#include "pch.h"
#include "TCPListener.h"
#include "Session.h"
void TCPListener::Init()
{
	m_listenSocket = WSASocketW(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (m_listenSocket == INVALID_SOCKET)
		throw "m_listenSocket IS INVALID";

	m_serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, IP, &m_serverAddr.sin_addr);
	m_serverAddr.sin_port = htons(PORT);
	int serverAddrLen = sizeof(m_serverAddr);

	if (::bind(m_listenSocket, (SOCKADDR*)&m_serverAddr, serverAddrLen) == INVALID_SOCKET)
		throw "Bind Error";

	if (::listen(m_listenSocket, 5) == SOCKET_ERROR)
		throw "Listten Error";
}

Session* TCPListener::Accept()
{
	SOCKADDR_IN clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	SOCKET clientSocket = accept(m_listenSocket, (SOCKADDR*)&clientAddr, &clientAddrLen);
	Session* newSession = m_sessionFactory();
	newSession->SetNetAddress(clientSocket, clientAddr);
	newSession->OnConnect();
	return newSession;
}

void TCPListener::Clean()
{
	::closesocket(m_listenSocket);
	WSACleanup();
}
