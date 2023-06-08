#include "pch.h"
#include "NetAddress.h"

NetAddress::NetAddress(const SOCKET& clientSocket, const SOCKADDR_IN& _clientAddr) : m_clientSockAddr(_clientAddr)
{
	int32 clinetAddrLen = sizeof(_clientAddr);
	SOCKADDR_IN tempSockAddr;
	::memset(&tempSockAddr, 0, clinetAddrLen);
	getpeername(clientSocket, (struct sockaddr*)&tempSockAddr, &clinetAddrLen);
	
	m_clientPort = ntohs(tempSockAddr.sin_port);
	InetNtopW(AF_INET, &tempSockAddr.sin_addr.s_addr, m_clientIp, sizeof(m_clientIp));
}
