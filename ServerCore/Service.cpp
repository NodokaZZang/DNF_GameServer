#include "pch.h"
#include "Service.h"
#include "TCPListener.h"
#include "IOCPCore.h"
#include "Session.h"
unsigned int __stdcall WorkThread(void* ptr);

Service::Service(IOCPCore* _core, TCPListener* _listener) : m_iocpCore(_core), m_tcpListener(_listener)
{
	Run();
}

Service::~Service()
{
	
}

void Service::Run()
{
	WSADATA		wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		throw "WSAStartUp";

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	int32 threadCount = sysInfo.dwNumberOfProcessors * 2;

	for (int i = 0; i < threadCount; i++) 
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &WorkThread, (void*)m_iocpCore, 0, NULL);

	m_tcpListener->Init();

	//TCPListener* listener = new TCPListener( new NetAddress("192.168.1.20", 30002) );
		
	while (true)
	{
		Session* newSession = m_tcpListener->Accept();
		m_iocpCore->RegisterIOCP(newSession);

		newSession->OnRead(0);
	}
}

unsigned int __stdcall WorkThread(void* ptr)
{
	IOCPCore* iocpCore = reinterpret_cast<IOCPCore*>(ptr);
	try {
		while (true)
			iocpCore->Dispatch();
	}
	catch (int expn)
	{
		int a = 3;
	}

	return 0;
}
