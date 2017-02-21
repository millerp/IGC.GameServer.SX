#include "stdafx.h"
#include "protocol.h"
#include "DamageExtension.h"
#include "AntiHack.h"
#include "ReconnectSystem.h"
#include "offsets.h"
#include "packetsend_hooks.h"
#include "HookManager.h"
#include "EncDec.h"
#include "giocp.h"
#include "ItemManagement.h"
#include "GensExt.h"

// global variables
char ChatServerIp[17];

//start of protocolcore
bool CliProtocolCore(LPBYTE aRecv, BYTE ProtoNum, int len, bool Encrypt)
{
	switch(ProtoNum)
	{
		case 0xF1:
			{
				PMSG_DEFAULT2 * lpMsg2 = (PMSG_DEFAULT2 *)aRecv;
				switch (lpMsg2->subcode)
				{
					case 0x00:
						GCJoinResultSend((PMSG_JOINRESULT*)aRecv);
						break;
				}
			}
			break;
		case 0xF3:
			{
				PMSG_DEFAULT2 * lpMsg2 = (PMSG_DEFAULT2 *)aRecv;
				switch (lpMsg2->subcode)
				{
				case 0x03:
					GCCharMapJoinResult((PMSG_CHARMAPJOINRESULT*)aRecv);
					break;
				case 0x04:
					GCCharRegen((PMSG_CHARREGEN*)aRecv);
					break;
				case 0x05:
					GCLevelUpSend((PMSG_LEVELUP*)aRecv);
					break;
				case 0x06:
					GCLevelUpPointAdd((PMSG_LVPOINTADDRESULT*)aRecv);
					break;
				case 0x51:
					GCMasterLevelUpSend((PMSG_MASTER_LEVEL_UP_SEND*)aRecv);
					break;
				}
			}
			break;
		case 0xBF:
		{
			PMSG_DEFAULT2 * lpMsg2 = (PMSG_DEFAULT2*)aRecv;
			switch (lpMsg2->subcode)
			{
			case 0x51:
				GCAnsMuBotRecvUse((PMSG_MUBOT_USE*)aRecv);
				break;
			}
		}
		break;
		case 0xF4:
			{
				// for reconnect must disable 0xF4 protocol or client crash
				BYTE internalProto = aRecv[3];
				if (g_ReconnectProcess >= 2 && g_ReconnectProcess != 6)
				{
					switch (internalProto)
					{
					case 0x03:
					{
						reconnectBuf = aRecv;
						g_ReconnectProcess = 3;

					}
					break;

					}
					return false;
				}
			}
			break;
		case 0xCA:
			GCFriendRoomCreate((PMSG_FRIEND_ROOMCREATE_RESULT*)aRecv);
			break;
		case 0xD4:
			AHCheckGetTickHook();
			break;
		case 0xFA:
		{
			if (aRecv[0] == 0xC1)
			{
				PMSG_DEFAULT2 * lpMsg2 = (PMSG_DEFAULT2 *)aRecv;
				switch (lpMsg2->subcode)
				{
				case 0x0D:
					GCOffTradeReq(aRecv);
					break;
				case 0xA0:
					GCEnableSiegeSkills();
					break;
				case 0xA2:
					GCSetAgilityFix((PMSG_SETAGILITYBUG*)aRecv);
					break;
				case 0xA3:
					GCSetCashItemMoveEnable();
					break;
				case 0xA4:
					HookThis((DWORD)&LuckyItemToolTipFix,0x00992239); // SXE3
					HookThis((DWORD)&LuckyItemEnhancementFix,0x00993F2D); // SXE3
					break;
				case 0xA5:
					GCDisableReconnect();
					break;
				case 0xA6:
					ExitProcess(0);
					break;
				case 0xA7:
					GCDisableReconnectSystem();
					break;
				case 0xA8:{
					PMSG_ALTERPSHOPVAULT * lpMsg = (PMSG_ALTERPSHOPVAULT*)aRecv;
					GCAlterPShopVault(lpMsg->type);}
					break;
				case 0x90:
					GCIGCStatsAdd((PMSG_ADDSTATS*)aRecv);
					break;
				case 0xB0:
					GCDropSellMod();
					break;
					
				}
				return false;
			}
			else if (aRecv[0] == 0xC2)
			{
				PWMSG_DEFAULT2 * lpMsg2 = (PWMSG_DEFAULT2*)aRecv;
				switch (lpMsg2->subcode)
				{
					case 0xA4:
						GCRareItemSetPrices((PMSG_RAREITEMPIRCESEND*)aRecv);
						break;
					case 0xF8:
						GCSetBattleZoneData((_tagPMSG_SEND_GENS_BATTLEZONE_DATA *)aRecv);
						break;
				}
				return false;
			}
		}
			break;
		case 0x11:
			GCAttackResult((PMSG_ATTACKRESULT*)aRecv);
			break;
		case 0x17:
			GCDiePlayerSend((PMSG_DIEPLAYER*)aRecv);
			break;
		case 0x26:
			GCRefillSend((PMSG_REFILL*)aRecv);
			break;
		case 0x27:
			GCManaSend((PMSG_MANASEND*)aRecv);
			break;
	}
	return true;
}

void GCDiePlayerSend(PMSG_DIEPLAYER *lpMsg)
{
	int aIndex = *(DWORD*)0x9DE8274; // aIndex SXE3

	int index = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if (index == aIndex)
	{
		hpDisplay = 0;
	}
}

void GCAttackResult(PMSG_ATTACKRESULT *lpMsg)
{
	

	int iIndex = *(DWORD*)0x9DE8274; // aIndex SXE3
	dmgDisplay = lpMsg->IGCDamage;

	int IndexH = lpMsg->NumberH;
	int IndexL = lpMsg->NumberL;

	IndexH &= 0x7F; // Delete MSB Flag

	int AttackIndex = MAKE_NUMBERW(IndexH, IndexL);

	if (AttackIndex == iIndex)
	{
		if (hpDisplay - dmgDisplay <= 0)
		{
			hpDisplay = 0;
		}
		else
		{
			hpDisplay -= dmgDisplay;
		}
	}

	

	

	BYTE check[5];
	DWORD dwAddr = (DWORD)GetProcedureAddress(GetModuleHandle("kernel32.dll"), "QueryPerformanceCounter");
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwAddr, check, 5, 0);

	if (memcmp(originalqueryperformancecounter, check, 5) != 0) // If recv is hooked by some shit
	{
		DWORD OldProtect;
		VirtualProtect((LPVOID)(0x00635438), 6, PAGE_EXECUTE_READWRITE, &OldProtect);
		BYTE FuckYou[6] = { 0x48, 0xC1, 0x46, 0x55, 0xEB, 0x22 };
		memcpy((void*)0x00635438, &FuckYou, 6); // Trap :: It will crash main on send packet :D
		GCSendAntihackBreach(10000);
	}

	
}

void GCRefillSend(PMSG_REFILL* lpMsg)
{
	if (lpMsg->IPos == 0xFE)
	{
		maxHPDisplay = lpMsg->IGCLife;
	}
	else if (lpMsg->IPos == 0xFF)
	{
		hpDisplay = lpMsg->IGCLife;
	}
	


	BYTE checksh[3];
	BYTE original[] = { 0x83, 0xE8, 0x28 };
	DWORD dwAddr = 0x004E453D; // SXE3
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwAddr, checksh, 3, 0);

	if (memcmp(checksh, original, 3) != 0)
	{
		DWORD OldProtect;
		VirtualProtect((LPVOID)(0x00552B5C), 6, PAGE_EXECUTE_READWRITE, &OldProtect);
		BYTE trapArray[6] = { 0xE8, 0x22, 0xA9, 0x99, 0x09, 0xEb };
		memcpy((void*)0x00552B5C, &trapArray, 6); // Trap :: crash on using potion
		GCSendAntihackBreach(60000);
	}

	
}

void GCManaSend(PMSG_MANASEND *lpMsg)
{
	if (lpMsg->IPos == 0xFE)
	{
		maxmanaDisplay = lpMsg->IGCMana;
	}
	else if (lpMsg->IPos == 0xFF)
	{
		manaDisplay = lpMsg->IGCMana;
	}

	

	/*DWORD dwExitCode;
	GetExitCodeThread(hCheckThread, &dwExitCode);

	if (dwExitCode != STILL_ACTIVE)
	{
		DWORD OldProtect;
		VirtualProtect((LPVOID)(0x009C51496), 6, PAGE_EXECUTE_READWRITE, &OldProtect);
		BYTE trapArray[6] = { 0xEB, 0x28, 0x16, 0x77, 0xAF, 0x4B };
		memcpy((void*)0x009C51496, &trapArray, 6); // Trap :: It will crash main on send packet :D
		GCSendAntihackBreach(20000);
	}*/

	
}

void GCJoinResultSend(PMSG_JOINRESULT *lpMsg)
{
	
	if (g_ReconnectProcess >= 4 && g_ReconnectProcess != 6)
	{
		g_ReconnectProcess = 5;
	}
	
}

void GCCharMapJoinResult(PMSG_CHARMAPJOINRESULT *lpMsg)
{
	hpDisplay = lpMsg->IGCLife;
	maxHPDisplay = lpMsg->IGCMaxLife;
	manaDisplay = lpMsg->IGCMana;
	maxmanaDisplay = lpMsg->IGCMaxMana;

	CGSendFilesCrc();
}

void GCCharRegen(PMSG_CHARREGEN * lpMsg)
{
	hpDisplay = lpMsg->IGCLife;
	manaDisplay = lpMsg->IGCMana;
}

void GCLevelUpSend(PMSG_LEVELUP *lpMsg)
{
	maxHPDisplay = lpMsg->IGCMaxLife;
	maxmanaDisplay = lpMsg->IGCMaxMana;
}

void GCLevelUpPointAdd(PMSG_LVPOINTADDRESULT *lpMsg)
{
	if (lpMsg->ResultType == 0x12)
	{
		maxHPDisplay = lpMsg->IGCMaxLifeAndMana;
	}

	else if (lpMsg->ResultType == 0x13)
	{
		maxmanaDisplay = lpMsg->IGCMaxLifeAndMana;
	}
}

void GCIGCStatsAdd(PMSG_ADDSTATS* lpMsg)
{
	WORD stats = lpMsg->Points;

	switch (lpMsg->statstype)
	{
	case 0:
		*(WORD*)((*(DWORD*)0x85F7168) + 0x118) = stats; // SXE3
		break;
	case 1:
		*(WORD*)((*(DWORD*)0x85F7168) + 0x11A) = stats; // SXE3
		break;
	case 2:
		*(WORD*)((*(DWORD*)0x85F7168) + 0x11C) = stats; // SXE3
		maxHPDisplay = lpMsg->MaxLifeAndMana;
		break;
	case 3:
		*(WORD*)((*(DWORD*)0x85F7168) + 0x11E) = stats; // SXE3
		maxmanaDisplay = lpMsg->MaxLifeAndMana;
		break;
	case 4:
		*(WORD*)((*(DWORD*)0x85F7168) + 0x120) = stats; // SXE3
		break;

	}

	*(WORD*)((*(DWORD*)0x85F7168) + 0x174) = lpMsg->LUP; // SXE3
}

void __fastcall GCOffTradeReq(BYTE* aRecv)
{
	g_UserQuit = TRUE;
	closesocket(*g_MuSocket);
}

void GCSetAgilityFix(PMSG_SETAGILITYBUG* lpMsg)
{
	DWORD OldProtect;
	VirtualProtect((LPVOID)0x00569CF9, 1, PAGE_EXECUTE_READWRITE, &OldProtect); // SXE3
	*(BYTE*)(0x00569CF9) = lpMsg->value;
}

void GCSetCashItemMoveEnable()
{
	return;

	BYTE btWareEnable[6] = { 0xE9, 0xDA, 0x00, 0x00, 0x00, 0x90 };
	BYTE btTradeEnable[6] = { 0xE9, 0x94, 0x00, 0x00, 0x00, 0x90 };

	DWORD OldProtect;
	VirtualProtect((LPVOID)0x009B5648, 6, PAGE_EXECUTE_READWRITE, &OldProtect);
	memcpy((LPVOID)0x009B5648, btWareEnable, sizeof(btWareEnable));

	VirtualProtect((LPVOID)0x009B4F86, 6, PAGE_EXECUTE_READWRITE, &OldProtect);
	memcpy((LPVOID)0x009B4F86, btTradeEnable, sizeof(btTradeEnable));

}
void GCMasterLevelUpSend(PMSG_MASTER_LEVEL_UP_SEND *lpMsg)
{
	maxHPDisplay = lpMsg->IGCMaxLife;
	maxmanaDisplay = lpMsg->IGCMaxMana;
}
void GCFriendRoomCreate(PMSG_FRIEND_ROOMCREATE_RESULT* lpMsg)
{
	strcpy(ChatServerIp, (const char*)lpMsg->ServerIp);
}

void GCEnableSiegeSkills()
{
	// Not work - function changed in main (probably read skill "category" from skill_xxx.bmd)
	// 5C03D3
	/*
	DWORD OldProtect;
	VirtualProtect((LPVOID)0x005B1C94, 50, PAGE_EXECUTE_READWRITE, &OldProtect); // S9
	*(BYTE*)(0x005B1C94 + 1) = 0x4A;
	*(BYTE*)(0x005B1C9A + 1) = 0x44;
	*(BYTE*)(0x005B1CA0 + 1) = 0x3E;
	*(BYTE*)(0x005B1CA6 + 1) = 0x38;
	*(BYTE*)(0x005B1CAC + 1) = 0x32;
	*(BYTE*)(0x005B1CB2 + 1) = 0x2C;
	*(BYTE*)(0x005B1CBB) = 0xEB;
	// all s9
	*/
}

void CGSendFilesCrc()
{
	PMSG_SEND_CRC msg;
	msg.h.c = 0xC1;
	msg.h.headcode = 0xFA;
	msg.h.size = sizeof(PMSG_SEND_CRC);
	msg.m_SubCode = 0x0D;


	char szPath[MAX_PATH];
	GetModuleFileName(GetCurrentModule(), szPath, MAX_PATH);
	msg.m_Crc[1] = g_EncDec->ComputeFileCRC("IGC.dll");

	GetModuleFileName(NULL, szPath, MAX_PATH);
	msg.m_Crc[0] = g_EncDec->ComputeFileCRC(szPath);

	//msg.m_Crc[0] = g_EncDec->ComputeFileCRC("Main.exe");
	//msg.m_Crc[1] = g_EncDec->ComputeFileCRC("IGC.dll");
	msg.m_Crc[2] = g_EncDec->ComputeFileCRC("Data\\Player\\Player.bmd");
	msg.m_Crc[4] = g_EncDec->ComputeFileCRC("Data\\Local\\Eng\\skill_eng.bmd");
	//msg.m_Crc[3] = g_ProcessScan->GetCrc();
	DataSend(&msg, sizeof(PMSG_SEND_CRC));
}

inline void GCSendAntihackBreach(DWORD dwErrorCode)
{
	
	PMSG_ANTIHACK_BREACH pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xFA;
	pMsg.subcode = 0x0A;
	pMsg.dwErrorCode = dwErrorCode;

	DataSend(&pMsg, pMsg.h.size);
	
}

void GCSetCharSet(WORD CharSet)
{
	// need to find offsets for season x e3
	/*
	DWORD OldProtect;
	VirtualProtect((LPVOID)0x00A18E07, 10, PAGE_EXECUTE_READWRITE, &OldProtect); //s9

	BYTE CHAT_FIX[] = { 0xB8, LOBYTE(CharSet), HIBYTE(CharSet), 0x00, 0x00, 0x90 };
	memcpy((LPVOID)0x00A18E07, CHAT_FIX, sizeof(CHAT_FIX)); //s9

	VirtualProtect((LPVOID)0x00DF46BB, 2, PAGE_EXECUTE_READWRITE, &OldProtect); //s9
	BYTE CHAT_ASIANFIX[] = { LOBYTE(CharSet), HIBYTE(CharSet) };
	memcpy((LPVOID)0x00DF46BB, CHAT_ASIANFIX, sizeof(CHAT_ASIANFIX)); // s9

	//s9
	VirtualProtect((LPVOID)0x00A18FCE, 2, PAGE_EXECUTE_READWRITE, &OldProtect);
	memcpy((LPVOID)0x00A18FCE, CHAT_ASIANFIX, sizeof(CHAT_ASIANFIX));

	//s9
	VirtualProtect((LPVOID)0x00A1912D, 2, PAGE_EXECUTE_READWRITE, &OldProtect);
	memcpy((LPVOID)0x00A1912D, CHAT_ASIANFIX, sizeof(CHAT_ASIANFIX));


	VirtualProtect((LPVOID)0x00A19128, 4, PAGE_EXECUTE_READWRITE, &OldProtect);

	DWORD CalcValue = 0x100000000 - CharSet;

	*(DWORD*)(0x00A19128) = CalcValue; //s9

	VirtualProtect((LPVOID)0x00A18D3E, 2, PAGE_EXECUTE_READWRITE, &OldProtect);
	memcpy((LPVOID)0x00A18D3E, CHAT_ASIANFIX, sizeof(CHAT_ASIANFIX)); // s9


	VirtualProtect((LPVOID)0x0051B6AA, 1, PAGE_EXECUTE_READWRITE, &OldProtect);
	*(BYTE*)(0x0051B6AA) = 0x6D; // s9

	MemSet(0x0045277A, 0x74, 1); // s9
	MemSet(0x00A18D0A, 0xEB, 1); // s9
	MemSet(0x00D5AE3E, 0xEB, 1); // s9

	MemSet(0x00438528, 0xEB, 1); // s9
	MemSet(0x00452744, 0xEB, 1); // S9 -- Asian support for range of input boxes

	BYTE charfix3[2] = { 0xEB, 0x48 };

	VirtualProtect((LPVOID)0x00452685, 2, PAGE_EXECUTE_READWRITE, &OldProtect);
	memcpy((LPVOID)0x00452685, charfix3, sizeof(charfix3)); // s9

	VirtualProtect((LPVOID)0x004B391C, 2, PAGE_EXECUTE_READWRITE, &OldProtect); //s9

	memcpy((LPVOID)0x004B391C, CHAT_ASIANFIX, sizeof(CHAT_ASIANFIX)); // s9
	*/
}

void GCRareItemSetPrices(PMSG_RAREITEMPIRCESEND * lpMsg)
{
	ModifyValueInt(JEWEL_OF_CHAOS_PRICE, lpMsg->iJewelOfChaosPrice);
	ModifyValueInt(JEWEL_OF_BLESS_PRICE, lpMsg->iJewelOfBlessPrice);
	ModifyValueInt(JEWEL_OF_SOUL_PRICE, lpMsg->iJewelOfSoulPrice);
	ModifyValueInt(JEWEL_OF_LIFE_PRICE, lpMsg->iJewelOfLifePrice);
	ModifyValueInt(JEWEL_OF_CREATION_PRICE, lpMsg->iJewelOfCreationPrice);
	ModifyValueInt(CREST_OF_MONARCH_PRICE, lpMsg->iCrestOfMonarchPrice);
	ModifyValueInt(LOCH_FEATHER_PRICE, lpMsg->iLochFeatherPrice);
	ModifyValueInt(JEWEL_OF_GUARDIAN_PRICE, lpMsg->iJewelOfGuardianPrice);
	ModifyValueInt(WERERABBIT_EGG_PRICE, lpMsg->iWereRabbitEggPrice);
	MemSet(ITEM_SELL_DIVISOR, lpMsg->btItemSellPriceDivisor,1 );
}

void GCDisableReconnect()
{
	g_UserQuit = 1;
	closesocket(*g_MuSocket);
}
void GCDisableReconnectSystem()
{
	g_UserQuit = 1;
}
void GCAnsMuBotRecvUse(PMSG_MUBOT_USE * aRecv)
{
	g_MuBotEnabled = aRecv->Status;
}

void CGEnableMUBot(int state)
{
	PMSG_MUBOT_SWITCH pMsg;

	pMsg.h.set((LPBYTE)&pMsg,0xBF,0x51,sizeof(pMsg));

	pMsg.Status = state;

	DataSend(&pMsg,sizeof(pMsg));
}

void GCAlterPShopVault(int type)
{
	switch(type)
	{
	case 0:
		pShopPriceTag = "Zen: %s";
		break;
	case 1:
		pShopPriceTag = "WCoinC: %s";
		break;
	case 2:
		pShopPriceTag = "WCoinP: %s";
		break;
	case 3:
		pShopPriceTag = "Goblin Point: %s";
		break;
	}
}
/* You are not allowed to drop this expensive item
005AFF17   E9 92000000      JMP main.005AFFAE
005AFF1C   90               NOP
*/

/* Reinforced item can't be sold.
00A1426E   E9 97000000      JMP main.00A1430A
00A14273   90               NOP
*/
void GCDropSellMod()
{
	DWORD OldProtect;
	BYTE btDropItem[6] = { 0xE9, 0x92, 0x00, 0x00, 0x00, 0x90 };
	BYTE btReinforcedSell[6] = {  0xE9, 0x97, 0x00, 0x00, 0x00, 0x90 };

	VirtualProtect((LPVOID)(0x005BE6E0), 6, PAGE_EXECUTE_READWRITE, &OldProtect); // SXE3
	VirtualProtect((LPVOID)(0x008AF44E), 6, PAGE_EXECUTE_READWRITE, &OldProtect); // SXE3

	memcpy((void*)0x005BE6E0, &btDropItem, 6); // SXE3
	memcpy((void*)0x008AF44E, &btReinforcedSell, 6); // SXE3
}