#pragma once
class IOCPCore;
class TCPListener;

class Service
{
private:
	IOCPCore*		m_iocpCore;
	TCPListener*	m_tcpListener;
public:
	Service(IOCPCore* _core, TCPListener* _listener);
	~Service();

private:
	void Run();
};

