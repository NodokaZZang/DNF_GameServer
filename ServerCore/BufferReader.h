#pragma once
class BufferReader
{
private:
	BYTE* m_buffer;
	int32 m_readPos = 0;
public:
	BufferReader(BYTE* _buffer);
	~BufferReader();

	template<typename T>
	void Read(T& data);

	void ReadWString(WCHAR* str, int32 strSize);

	int32 GetReadSize() { return m_readPos; }
};

template<typename T>
inline void BufferReader::Read(T& data)
{
	int typeSize = sizeof(T);
	data = *(T*) & m_buffer[m_readPos];
	m_readPos += typeSize;
}
