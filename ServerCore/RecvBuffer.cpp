#include "pch.h"
#include "RecvBuffer.h"

bool RecvBuffer::OnRead(int32 numOfBytes, BYTE** dataPtr, int32& dataLen)
{
	if (m_writePos + numOfBytes > RECV_BUFFER_CHUNK * 4)
	{
		int32 dataSize = GetDataSize() + numOfBytes;
		::memcpy(m_buffer.data(), &m_buffer[m_readPos], dataSize);
		m_writePos = GetDataSize();
		m_readPos = 0;
	}

	m_writePos += numOfBytes;

	if (GetDataSize() < sizeof(PacketHeader))
		return false;

	PacketHeader* header = reinterpret_cast<PacketHeader*>(GetReadPos());

	if (GetDataSize() < header->_pktSize)
		return false;

	*dataPtr = GetReadPos();
	dataLen = header->_pktSize;

	return true;
}

void RecvBuffer::AddRecvPos(int32 dataLen)
{
	m_readPos += dataLen;

	if (m_readPos == m_writePos)
	{
		m_readPos = 0;
		m_writePos = 0;
	}
}
