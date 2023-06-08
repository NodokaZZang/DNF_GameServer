#pragma once
class Session;

class IOCPCore
{
private:
	HANDLE	m_cpObject;

public:
	IOCPCore();
	~IOCPCore();

	void RegisterIOCP(Session* _session);
	void Dispatch();
};

