#include "pch.h"
#include "Session.h"
#include "NetAddress.h"

Session::~Session()
{
	delete m_netAddress;
}

void Session::OnRead(int32 numOfBytes)
{
	while (true)
	{
		BYTE* dataPtr = nullptr;
		int32 dataLen = 0;

		if (m_recvBuffer.OnRead(numOfBytes, &dataPtr, dataLen))
		{
			OnRecv(this,dataPtr, dataLen);
			m_recvBuffer.AddRecvPos(dataLen);
			numOfBytes = 0;
		}
		else
		{
			break;
		}
	}

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)m_recvBuffer.GetWritePos();
	wsaBuf.len = m_recvBuffer.GetFreeSize();
	DWORD flag = 0;
	DWORD newNumOfBytes = 0;

	if (WSARecv(m_clientSocket, &wsaBuf, 1, &newNumOfBytes, &flag, &m_overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			wprintf(L"RecvError %s \n", m_netAddress->GetClientIPSTR());
		}
	}
}

void Session::OnRecv(Session* session, BYTE* dataPtr, int32 dataLen)
{
	wprintf(L"onRecv %s: size: %d\n", m_netAddress->GetClientIPSTR(), dataLen);
}

void Session::OnDisconnect()
{
	wprintf(L"onDisconnect %s \n", m_netAddress->GetClientIPSTR());
}

void Session::OnConnect()
{
	wprintf(L"onConnect %s \n", m_netAddress->GetClientIPSTR());
}

void Session::SetNetAddress(SOCKET _socket, SOCKADDR_IN& _sockAddrIn) 
{
	m_clientSocket = _socket;
	m_netAddress = new NetAddress(_socket, _sockAddrIn);
}

void Session::Send(BYTE* _buffer, int32 _bufferLen)
{
	::send(m_clientSocket, (char*)_buffer, _bufferLen, 0);
}