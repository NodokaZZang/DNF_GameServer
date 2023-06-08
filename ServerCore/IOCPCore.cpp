#include "pch.h"
#include "IOCPCore.h"
#include "Session.h"
#include "NetAddress.h"
IOCPCore::IOCPCore()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    int32 threadCount = sysInfo.dwNumberOfProcessors * 2;

    m_cpObject = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, threadCount);
}

IOCPCore::~IOCPCore()
{
    CloseHandle(m_cpObject);
}

void IOCPCore::RegisterIOCP(Session* _session)
{
    CreateIoCompletionPort((HANDLE)_session->GetSocket(), m_cpObject, reinterpret_cast<ULONG_PTR>(_session), 0);
}

void IOCPCore::Dispatch()
{
    DWORD numOfBytes = 0;
    LPOVERLAPPED overlapped;
    Session* session;

    bool ret = GetQueuedCompletionStatus(m_cpObject, &numOfBytes, (PULONG_PTR)&session, &overlapped, INFINITE);

    if (ret == false)
    {
        int32 errCode = WSAGetLastError();
        std::cout << errCode << std::endl;
        closesocket(session->GetSocket());
        session->OnDisconnect();
        delete session;
    }

    else if (numOfBytes == 0)
    {
        closesocket(session->GetSocket());
        session->OnDisconnect();
        delete session;
    }

    else
    {
        session->OnRead(numOfBytes);
    }
}
