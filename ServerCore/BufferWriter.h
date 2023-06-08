#pragma once
class BufferWriter
{
private:
	BYTE* m_buffer;
	int32 m_writePos = 0;
public:
	BufferWriter(BYTE* _buffer);
	~BufferWriter();

	template<typename T>
	T* WriteReserve();

	template<typename T>
	void Write(T data);

	void WriteWString(WCHAR* str, int32 strSize);

	int32 GetWriterSize() {return m_writePos;}

};

template<typename T>
inline T* BufferWriter::WriteReserve()
{
	T* ret = reinterpret_cast<T*>(m_buffer);
	m_writePos += sizeof(T);
	return ret;
}

template<typename T>
inline void BufferWriter::Write(T data)
{
	int dataSize = sizeof(T);
	::memcpy(&m_buffer[m_writePos], &data, dataSize);
	m_writePos += dataSize;
}
