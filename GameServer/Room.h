#pragma once
class Room
{
private:
	int32 m_roomSq = 0;
	WCHAR m_title[30] = {0};
	int32 m_joinCnt = 0;
public:
	Room();
	~Room();

	void SetTitle(WCHAR* _title, int32 _strSize);
	void SetTitle(WCHAR* _title);
	WCHAR* GetTitle() { return m_title; }
};

