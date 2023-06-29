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
    C_T_S_CREATEROOM,
    C_T_S_ENTEROOM,
    S_T_C_ENTEROOM,
    C_T_S_WATINGROOMINIT,
    S_T_C_WATINGROOMINIT,
    C_T_S_WATINGROOMOUTUSER,
    S_T_C_WATINGROOMOUTUSER,
    C_T_S_CHATSEND_WATINGROOM,
    S_T_S_CHATSEND_WATINGROOM,
    C_T_S_USERPICK,
    C_T_S_USERREADY,
    S_T_C_WATINGROOMADMIN,
    S_T_C_GAMESTART,
    C_T_S_GAMEINIT,
    S_T_C_GAMEINIT,
    S_T_C_GAMESTAGEINIT,
    C_T_S_PLAYERPOSSYNC,
    S_T_C_PLAYERPOSSYNC,
    C_T_S_MONSTERSYNC,
    S_T_C_MONSTERSYNC,
    C_T_S_PLAYERATTACK,
    S_T_C_PLAYERATTACK,
    UDP_PLAYERSYNC,
    C_T_S_UDPIPPORTSEND,
    UDP_MONSTERSPAWN,
    UDP_MONSTERSYNC,
    UDP_MONSTERATTACKED,
    UDP_PLAYERATTACKED,
    C_T_S_GAMEEND,
};