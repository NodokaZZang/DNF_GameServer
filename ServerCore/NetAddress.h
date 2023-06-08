#pragma once
class NetAddress
{
private:
	SOCKADDR_IN m_clientSockAddr;
	WCHAR		m_clientIp [15] = {0};
	uint32		m_clientPort;
public:
	NetAddress(const SOCKET& clientSocket, const	SOCKADDR_IN& _clientAddr);
	~NetAddress() {};

	SOCKADDR_IN& GetSockAddr() { return m_clientSockAddr; }
	WCHAR* GetClientIPSTR() { return m_clientIp; }
	uint32 GetClientPort() { return m_clientPort; }
};

