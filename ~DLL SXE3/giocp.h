#ifndef GIOCP_H
#define GIOCP_H
// defines
#define PARSE_PACKET_HOOK 0x00A10CC6 // SXE3
#define PARSE_PACKET_STREAM 0x009E0D29 // SXE3
#define PROTOCOL_CORE1 0x00A1CB86 // SXE3
#define PROTOCOL_CORE2 0x00A18E41 // SXE3

#define SEND_PACKET_HOOK 0x009DFE3E // SXE3
#define MU_SEND_PACKET 0x009E0265 // SXE3
#define MU_SENDER_CLASS 0x12AD724 // SXE3

// function headers

void ParsePacket(void* PackStream, int unk1, int unk2);
void DataSend(void* pData, int size);
void SendPacket(BYTE* buff, int len, int enc, int unk1);
extern SOCKET* g_MuSocket;

#endif