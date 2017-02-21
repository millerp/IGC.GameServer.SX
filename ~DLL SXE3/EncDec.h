#ifndef ENCDEC_H
#define ENCDEC_H



static unsigned char bBuxCode[3] = { 0xFC, 0xCF, 0xAB };	// Xox Key for some interesthing things :)
class CEncDec {
	friend class CAntiSpeed;
	bool m_CGInitialized;
	bool m_GCInitialized;

	BYTE* m_CGXorKey;
	BYTE* m_CGSubKey;

	BYTE* m_GCXorKey;
	BYTE* m_GCSubKey;
public:

	DWORD __fastcall ComputeCRC(BYTE* data, int size);
	DWORD __fastcall ComputeFileCRC(char* file);
	static void EncXor32(unsigned char*Buff, int SizeOfHeader, int Len);
	static void DecXor32(unsigned char*Buff, int SizeOfHeader, int Len);
	static void BuxConvert(char* buf, int size);
};
extern CEncDec* g_EncDec;
#endif