#include "pch.h"
#include "BufferReader.h"

BufferReader::BufferReader(BYTE* _buffer) : m_buffer(_buffer)
{
}

BufferReader::~BufferReader()
{
}

void BufferReader::ReadWString(WCHAR* str, int32 strSize)
{
	::memcpy(str, &m_buffer[m_readPos], strSize);
	m_readPos += strSize;
}
