#pragma once
class NetAddress;
#include "RecvBuffer.h"
class Session
{
protected:
	SOCKET			m_clientSocket;
	NetAddress*		m_netAddress;
	WSAOVERLAPPED	m_overlapped;
	RecvBuffer		m_recvBuffer;
	WCHAR			m_username[30] = {0};
	
public:
	Session() {};
	~Session();

	SOCKET&			GetSocket() { return m_clientSocket; }
	void			OnRead(int32 numOfBytes);
	NetAddress*		GetNetAddr() { return m_netAddress; }
	void			SetNetAddress(SOCKET _socket, SOCKADDR_IN& _sockAddrIn);
	void			SetUsername(WCHAR* _username) { wcscpy_s(m_username, _username); };
	WCHAR*			GetUsername() { return m_username; }
	void			Send(BYTE* _buffer, int32 _bufferLen);

public:
	virtual void	OnRecv(Session* session, BYTE* dataPtr, int32 dataLen);
	virtual void	OnDisconnect();
	virtual void    OnConnect();
};

