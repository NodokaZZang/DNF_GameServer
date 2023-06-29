#pragma once
class Session;
class PacketHandler
{
public:
	static void PakcetHandle(Session* _session, BYTE* _dataPtr, int _dataSize);

private:
	static void PakcetHandle_C_T_S_LOGIN(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PakcetHandle_C_T_S_USERLIST(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PakcetHandle_C_T_S_ROOMLIST(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PakcetHandle_C_T_S_CHATSEND(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PakcetHandle_C_T_S_CREATEROOM(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PakcetHandle_C_T_S_ENTEROOM(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PacketHandle_C_T_S_WATINGROOMINIT(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PacketHandle_C_T_S_WATINGROOMOUTUSER(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PacketHandle_C_T_S_CHATSEND_WATINGROOM(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PacketHandle_C_T_S_USERPICK(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PacketHandle_C_T_S_USERREADY(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PacketHandle_C_T_S_GAMEINIT(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PacketHandle_C_T_S_UDPIPPORTSEND(Session* _session, BYTE* _dataPtr, int _dataSize);
	static void PacketHandle_C_T_S_GAMEEND(Session* _session, BYTE* _dataPtr, int _dataSize);
};

