#pragma once
#define RECV_BUFFER_CHUNK 4096
#define RECV_BUFFER_COUNT 5
#define RECV_BUFFER_LEN 4096 * RECV_BUFFER_COUNT
class RecvBuffer
{
private:
	std::vector<BYTE> m_buffer;
	int32 m_readPos = 0;
	int32 m_writePos = 0;
	int32 m_bufferSize = RECV_BUFFER_LEN;
public:

	RecvBuffer() { m_buffer.resize(RECV_BUFFER_LEN, 0); }

	int32 GetDataSize() { return m_writePos - m_readPos; }
	int32 GetFreeSize() { return m_bufferSize - m_writePos; };
	BYTE* GetBuffer() { return m_buffer.data(); }
	BYTE* GetReadPos() { return &m_buffer[m_readPos]; }
	BYTE* GetWritePos() { return &m_buffer[m_writePos]; }
	bool  OnRead(int32 numOfBytes, BYTE** dataPtr, int32& dataLen);
	void  AddRecvPos(int32 dataLen);
};

