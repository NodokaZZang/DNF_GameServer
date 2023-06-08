#pragma once
class Session;
class PacketHandler
{
public:
	static void PakcetHandle(Session* _session, BYTE* _dataPtr, int _dataSize);

private:
	static void PakcetHandle_C_T_S_LOGIN(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PakcetHandle_C_T_S_USERLIST(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PakcetHandleC_T_S_ROOMLIST(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PakcetHandleC_T_S_CHATSEND(Session* _session, BYTE* _dataPtr, int _dataSize);
};

