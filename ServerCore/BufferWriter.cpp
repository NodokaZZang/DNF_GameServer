#include "pch.h"
#include "BufferWriter.h"

BufferWriter::BufferWriter(BYTE* _buffer) : m_buffer(_buffer)
{
	
}

BufferWriter::~BufferWriter()
{
}

void BufferWriter::WriteWString(WCHAR* str, int32 strSize) 
{
	WCHAR* startStr = reinterpret_cast<WCHAR*>(&m_buffer[m_writePos]);
	::memcpy(startStr, str, strSize);
	m_writePos += strSize;
}