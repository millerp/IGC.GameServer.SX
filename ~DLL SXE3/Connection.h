#ifndef CONNECTION_H
#define CONNECTION_H

#define MU_CONNECT_FUNC 0x009E27D6 // SXE3
#define CONNECT_HOOK1 0x004E19C7 // SXE3
#define CONNECT_HOOK2 0x0043E991 // SXE3
#define GSCONNECT_HOOK1 0x0A7B0BD2 // SXE3
#define GSCONNECT_HOOK2 0x00506C7B // SXE3
#define CHATSERVER_PORT 0x004BB5D1+1 // SXE3

enum CONNECTION_TYPE
{
	NOT_CONNECTED = 0,
	CS_CONNECTED,
	GS_CONNECTED,
};

typedef void (*tmuConnectToCS)(char* hostname, int port);
extern tmuConnectToCS muConnectToCS;

void OnConnect();
void OnGsConnect();

extern CONNECTION_TYPE g_Connection;
#endif