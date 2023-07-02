#pragma once
class UDPServer
{
private:
	SOCKET _socket;
	SOCKADDR_IN _serverAddrIn;

public:
	void Init();
	void Recv();
};

