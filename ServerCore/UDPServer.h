#pragma once
class UDPServer
{
private:
	SOCKET _socket;
	SOCKADDR_IN _serverAddrIn;
	std::list<SOCKADDR_IN> _udpSockAddrList;


public:
	void Init();
	void Recv();
};