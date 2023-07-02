#pragma once
class IOCPCore;
class TCPListener;
class UDPServer;
class Service
{
private:
	IOCPCore*		m_iocpCore;
	TCPListener*	m_tcpListener;
	UDPServer*	    m_udpServer;
public:
	Service(IOCPCore* _core, TCPListener* _listener);
	~Service();

private:
	void Run();
};

