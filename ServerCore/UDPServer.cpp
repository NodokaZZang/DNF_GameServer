#include "pch.h"
#include "UDPServer.h"
#include "NetAddress.h"
#include "BufferWriter.h"
#include "BufferReader.h"

unsigned int __stdcall UDPRecvThread(void* ptr);

void UDPServer::Init()
{
	_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (_socket == INVALID_SOCKET)
	{
		throw "_socket IS INVALID";
	}

	::memset(&_serverAddrIn, 0, sizeof(_serverAddrIn));
	_serverAddrIn.sin_family = AF_INET;
	inet_pton(AF_INET, IP, &_serverAddrIn.sin_addr);
	_serverAddrIn.sin_port = htons(30003);

	if (bind(_socket, (SOCKADDR*)&_serverAddrIn, sizeof(_serverAddrIn)) == SOCKET_ERROR)
	{
		throw "UDP BIND ERROR";
	}

	HANDLE recvThread = (HANDLE)_beginthreadex(NULL, 0, &UDPRecvThread, (void*)this, 0, NULL);
}

void UDPServer::Recv()
{
	int recvSize;
	char recvData[4028] = { 0 };
	SOCKADDR_IN clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	
	while (true) 
	{
		recvSize = recvfrom(_socket, recvData, sizeof(recvData), 0, (SOCKADDR*)&clientAddr, &clientAddrSize);
		WCHAR clientIP[25] = { 0 };
		InetNtopW(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
		int32 clientPort = ntohs(clientAddr.sin_port);

		wprintf(L"Connect UDP IP: %s Port: %d \n", clientIP,  clientPort);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(recvData);

		switch (header->_type)
		{
		case 39:
		{
			BYTE sendBuffer[1024];
			BufferWriter bw(sendBuffer);
			PacketHeader* pktHeader = bw.WriteReserve<PacketHeader>();
			int32 strSize = wcslen(clientIP) * 2;

			bw.Write(strSize);
			bw.WriteWString(clientIP, strSize);
			bw.Write(clientPort);

			pktHeader->_type = 40; // UDP_IPPORTINFO
			pktHeader->_pktSize = bw.GetWriterSize();
			sendto(_socket, (char*)sendBuffer, pktHeader->_pktSize, 0, (sockaddr*)&clientAddr, clientAddrSize);
			break; // C_T_S_IPPORTINFO,
		}

		case 41:
		{
		
			break;
		}
		}
	}
}

unsigned int __stdcall UDPRecvThread(void* ptr)
{
	UDPServer* udpServer = reinterpret_cast<UDPServer*>(ptr);
	try {
		udpServer->Recv();
	}
	catch (int expn)
	{
		int a = 3;
	}

	return 0;
}