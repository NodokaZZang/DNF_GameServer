#pragma once
enum PacketProtocol : __int16
{
	C_T_S_LOGIN,
	C_T_S_USERLIST,
	C_T_S_ROOMLIST,
	S_T_C_USERLIST,
	S_T_C_ROOMLIST,
	S_T_C_LOGIN,
	C_T_S_CHATSEND_LOBBY,
	S_T_C_CHATSEND_LOBBY,
};