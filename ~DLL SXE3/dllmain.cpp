// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Connection.h"
#include "offsets.h"
#include "HookManager.h"
#include "ItemManagement.h"
#include "DamageExtension.h"
#include "ResourceManager.h"
#include "ServerInfo.h"
#include "ItemLoad.h"
#include "packetsend_hooks.h"
#include "AntiHack.h"
#include "Interface.h"
//#include "Camera.h"
#include "WndHook.h"
#include "Macros.h"
#include "Glow.h"
#include "mdump.h"
#include "ReconnectSystem.h"
#include "protocol.h"
#include "giocp.h"
#include "Crack.h"
#include "GensExt.h"
#include "Controller.h"
#include "EventEntryLevel.h"

char IP[49];
WORD Port;
char Serial[17];

BYTE hookedrecv[5] = { 0 };
BYTE originalrecvheader[5] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };

BYTE hookedwglswap[5] = { 0 };
BYTE originalwglswapheader[5] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };

BYTE originalvirtualprotectex[5] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };

typedef HMODULE (WINAPI *LoadLibraryFPtr)(LPCTSTR dllName);
LoadLibraryFPtr origFunc = LoadLibraryA;
LoadLibraryFPtr mineFunc = NULL;
DWORD dwAntiHackTime = 0;



void __declspec(naked) onCsHook()
{
	//004DE303   50               PUSH EAX
//	004DE304   E8 68701500      CALL main_IGC.00635371
	OnConnect();

	_asm
	{
		push eax;
		//call 0x00635371; // it's not needed, 635371 is original Connect function, we use muConnectToCS
		mov edx ,0x004E19CD; // SXE3
		jmp edx;


	}

}
HMODULE WINAPI LoadLibraryMine(LPCTSTR dllName)
{
	return 0;
}




void WriteMem(HANDLE hProcess, DWORD addr, DWORD mem, DWORD size)
{
	DWORD OldProtect;
	VirtualProtectEx(hProcess, (LPVOID)addr, size, PAGE_EXECUTE_READWRITE, &OldProtect);
	SIZE_T bytes = size;
	WriteProcessMemory(hProcess, (LPVOID)addr, (LPVOID)mem, size, &bytes);

	VirtualProtectEx(hProcess, (LPVOID)addr, size, OldProtect, &OldProtect);
}

void SetHook()
{
	HookThis((DWORD)&ParsePacket, PARSE_PACKET_HOOK); // SXE3
	HookThis((DWORD)&SendPacket, SEND_PACKET_HOOK); // SXE3
	HookThis((DWORD)sprintf, 0x00D90F4A); // SXE3
	//HookThis((DWORD)&dmgSendHook, 0x00645F40); // S9 NOT WORK
	//HookThis((DWORD)&dmgSendRFHook, 0x00645B63); // S9 NOT WORK
	//HookThis((DWORD)&manaSendHook, 0x009CB1DD); // S9 NOT WORK
	//HookThis((DWORD)&HPSendHook, 0x009CB186); // S9 NOT WORK
	//HookThis((DWORD)&ManaSendCHook, 0x009AB287); // S9 NOT WORK
	//HookThis((DWORD)&ManaSendDKHook, 0x009AB251); // S9 NOT WORK
	//HookThis((DWORD)&HPSendDKHook, 0x009AAEE6); // S9 NOT WORK
	//HookThis((DWORD)&HPSendCHook, 0x009AAF1C); // S9 NOT WORK
	//HookThis((DWORD)&FixSkill65k,0x0092DA76); // S9 NOT WORK
	//HookThis((DWORD)&ErrtelMixStaffFix,0x009E3824); // S9 NOT USED - NEW PENTAGRAM MIX IN SEASON X E3
	//HookThis((DWORD)&ItemLevelReqFix, 0x00892F88); // SXE3
	//AlterPShopDisplayCurrency()
	//HookThis((DWORD)&AlterPShopDisplayCurrency, 0x0074AE61); // SXE3
	//g_Camera->Init();
	//ChangeAddress(MU_WND_PROC_HOOK, FPTR(WndProc));

	// HD Resolution
	//HookManager.MakeJmpHook(RESOLUTION_HOOK, MuLoadResolutionAsm);
	//GCSetCharSet(1255);
	//00591530 wing
	//0099500B
	//0063D3E3   51               PUSH ECX

	//0059E68B
}
void SpeedCheckThread()
{

	while(true){
	
//	static DWORD tick = GetTickCount();
/*	while ((GetTickCount() - tick) < 20000) 
		{
			Sleep(10);
		}
	/*
		while ((GetTickCount() - tick) < 20000) 
		{
			Sleep(10);
		}
		if(*g_Map != -1 || *g_Map != 0x4a || *g_Map != 0x49 || *g_Map != 0x4E )
		{
			CGSendAgi(0);
		}

		tick = GetTickCount();*/
	}

}
/*void LoadNewItems()
{
//	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SpeedCheckThread, NULL, NULL, NULL);
}*/
void SetValues()
{
	DWORD OldProtect;

	ResourceManager.Preloader();
	//g_Controller.Load();
	g_ServerInfo->Load("./Data/Local/ServerInfo.bmd");




	
	//GCSetCharSet(g_ServerInfo->GetCharset());
	HookManager.MakeJmpHook(CONNECT_HOOK1, onCsHook);
	MemSet(CONNECT_HOOK2, 0x90, 6);
	HookManager.MakeCallback(CONNECT_HOOK2, OnConnect, 0, 6, false);
	ChangeAddress(VERSION_HOOK1, (DWORD)g_ServerInfo->GetVersion());
	ChangeAddress(SERIAL_HOOK1, (DWORD)g_ServerInfo->GetSerial());
	ChangeAddress(SERIAL_HOOK2, (DWORD)g_ServerInfo->GetSerial());
	ChangeAddress(VERSION_HOOK2, (DWORD)g_ServerInfo->GetVersion());
	ChangeAddress(VERSION_HOOK3, (DWORD)g_ServerInfo->GetVersion());
	ModifyValueInt(CHATSERVER_PORT,(DWORD)g_ServerInfo->GetChatServerPort());
	HookManager.MakeCallback(GSCONNECT_HOOK1, OnGsConnect, 0, 10, false);
	HookManager.MakeCallback(GSCONNECT_HOOK2, OnGsConnect, 0, 10, false);
	/*MemSet(CTRL_FREEZE_FIX, 0x02, 1);
	MemSet(MAPSRV_DELACCID_FIX, 0x90, 5); // nop call to memcpy in mapserver, because buff source is empty, due to no account id from webzen http (remove new login system)
	HookManager.MakeCallback(RENDER_HP_BAR_CALLBACK, RenderLoadingBar, 0, 6, true);
	VM_TIGER_BLACK_END

	HookManager.MakeJmpHook(0x004E49F4, HookDCFunc); //SXE3

	HookManager.MakeJmpHook(0x0072962F, HookExitFunc); // SXE3
	HookManager.MakeJmpHook(0x0043F392,HookExitCharSelectFunc); // SXE3

	HookManager.MakeJmpHook(0x00CDF84B, MoveWndFix); // SXE3

	ResourceManager.LoadGraphics();

	HookManager.MakeJmpHook(SETGLOW_EX, OnSetGlowEx);

	MemSet(0x0089496E, 0x90, 2); // Exc socket item FIX, SXE3
	MemSet(0x00893ED6, 0xEB, 1); // Exc Socket Visual Bug Option FIX, SXE3

	MemSet(0x009E87FD, 0x90, 3); // S9 Fenrir exc visual FIX 1, SXE3
	MemSet(0x009E8819, 0x90, 2); // S9 Fenrir exc visual FIX 2, SXE3
	MemSet(0x009E8831, 0xEB, 1); // S9 Fenrir exc visual FIX 3, SXE3

	VirtualProtect((LPVOID)CHARSET_ADDRESS, 4, PAGE_EXECUTE_READWRITE, &OldProtect);
	*(DWORD*)CHARSET_ADDRESS = g_ServerInfo->GetCharset();
	// need to be fixed
	//MemSet(0x00438528, 0xEB, 1); // S9 -- JE -> JMP, Opens support of Asian Symbols in character creation
	//MemSet(0x00452685, 0xEB, 1); // S9 -- IME and Virtual inputs support
	//MemSet(0x00452686, 0x48, 1); // S9 -- IME and Virtual inputs support
	//MemSet(0x00452744, 0xEB, 1); // S9 -- Asian support for range of input boxes

	MemSet(0x009432B4,0xEB,1); // SXE3 -- Cherry Blossom Opening MuRuumy Windows Fix, JNZ -> JMP
	MemSet(0x0074C0C9,0x90,6); // SXE3 -- Joh option display on Ancients
	MemSet(0x009B14E3 + 3, 0x30, 1); // SXE3 -- Decreased limit of screen refresh frequency, 60 -> 48
	MemSet(0x009B14E7, 0x7D, 1); // SXE3 // JE -> JGE -- limit check == -> >=
	
	//RFSkillDamageDisplayFix(); // NOT WORK IN SEASON X

	GensInitData();
	HookManager.MakeJmpHook(0x005DBCE8, GensIsBattleZoneMap); // SXE3
	HookManager.MakeJmpHook(0x0092D266, GensWarpMenuFix); // SXE3
	MemSet(0x0092D1B1, 0x85, 1); // SXE3

	// DISABLED DUE TO NEW BC/DS/CC IN SEASON X
	//LoadBCEntryLevel(); // private custom !!!
	//LoadDSEntryLevel(); // private custom !!!


	// Launcher init
	SYSTEMTIME Date;
	GetLocalTime(&Date);
	DWORD dwParam = (Date.wDay * Date.wMonth * Date.wYear) ^ 0x12894376 + 0xFFFFFFF;
	if (g_ServerInfo->GetLauncher())
	{
		LPSTR cmdline = GetCommandLine();

		int len = strlen(cmdline);
		int i;
		for (i = (len - 1); i >= 0; i--)
		{
			if (cmdline[i] == ' ')
				break;
		}

		char* parameter = &cmdline[i + 1];
		int paramLen = strlen(parameter);
		bool isNum = true;
		for (int i = 0; i < paramLen; i++)
		{
			if (parameter[i] < '0' || parameter[i] > '9')
				isNum = false;
		}
		DWORD dwParameter = 0;
		if (isNum)
		{
			dwParameter = atoi(parameter);
		}

		if (dwParam != dwParameter)
		{
			WinExec(g_ServerInfo->GetLauncherFile().c_str(), SW_SHOW);
			ExitProcess(0);
		}
	}

	// item
	// Not work, need to be remade
	//HookManager.MakeJmpHook(TOOLTIP_FIX, ToolTipFix);
	*/
}

void RecvCheckThread()
{
	
	BYTE check[5];
	PMSG_ANTIHACK_CHECK pMsg;
	
	while (true)
	{
		DWORD dwAddr = (DWORD)GetProcedureAddress(GetModuleHandle("kernel32.dll"), "VirtualProtectEx");

		if (dwAddr == NULL)
		{
			int Error = GetLastError();
			char szTemp[128];
			sprintf(szTemp, "ERROR :%d", Error);
			MessageBox(0, szTemp, "ERROR", MB_OK);
		}

		ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwAddr, check, 5, 0);

		if (memcmp(originalvirtualprotectex, check, 5) != 0) // If recv is hooked earlier
		{
			DWORD OldProtect;
			VirtualProtect((LPVOID)(0x0116D0BB), 6, PAGE_EXECUTE_READWRITE, &OldProtect);
			BYTE FuckYou[6] = { 0xEB, 0x44, 0x27, 0xA1, 0x04, 0xDE };
			memcpy((void*)0x0116D0BB, &FuckYou, 6); // Trap :: It will crash main on send packet :D
			GCSendAntihackBreach(40000);
		} 

		/* dwAddr = (DWORD)GetProcedureAddress(GetModuleHandle("opengl32.dll"), "wglSwapBuffers");
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwAddr, check, 5, 0);

		 if (memcmp(originalwglswapheader, check, 5) != 0) // If recv is hooked by some shit
		{
			DWORD OldProtect;
			VirtualProtect((LPVOID)(0x0043937C), 6, PAGE_EXECUTE_READWRITE, &OldProtect);
			BYTE FuckYou[6] = { 0xE8, 0x9B, 0xE9, 0x98, 0x22, 0xC1 };
			memcpy((void*)0x0043937C, &FuckYou, 6); // Trap :: It will crash main on send packet
			GCSendAntihackBreach(60000);
		} */

		AHCheckSwapBufferMod();

		if (GetTickCount() - dwAntiHackTime >= 20000 && g_Connection == GS_CONNECTED)
		{
			pMsg.h.c = 0xC1;
			pMsg.h.size = sizeof(pMsg);
			pMsg.h.headcode = 0xFA;
			pMsg.subcode = 0x11;
			
			dwAddr = (DWORD)GetProcedureAddress(GetModuleHandle("ws2_32.dll"), "recv");
			ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwAddr, pMsg.checkdata, 5, 0);

			DataSend((LPBYTE)&pMsg, pMsg.h.size);

			dwAntiHackTime = GetTickCount();
		}

		Sleep(1000);
	}

	
}

void CheckMainFunctions()
{
	

	BYTE check[5];

	DWORD dwAddr = (DWORD)GetProcedureAddress(GetModuleHandle("kernel32.dll"), "VirtualProtectEx");
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwAddr, check, 5, 0);

	if (memcmp(originalvirtualprotectex, check, 5) != 0) // If recv is hooked earlier
	{
		DWORD OldProtect;
		VirtualProtect((LPVOID)(0x0116D0BB), 6, PAGE_EXECUTE_READWRITE, &OldProtect);
		BYTE FuckYouLG[6] = { 0xEB, 0x44, 0x27, 0xA1, 0x04, 0xDE };
		memcpy((void*)0x0116D0BB, &FuckYouLG, 6); // Trap :: It will crash main on send packet
	}

	dwAddr = (DWORD)GetProcedureAddress(GetModuleHandle("ws2_32.dll"), "recv");
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)dwAddr, check, 5, 0);

	if (memcmp(originalrecvheader, check, 5) != 0) // If recv is hooked earlier
	{
		DWORD OldProtect;
		VirtualProtect((LPVOID)(0x0043937C), 6, PAGE_EXECUTE_READWRITE, &OldProtect);
		BYTE FuckYouLG[6] = {0xE8, 0x9B, 0xE9, 0x98, 0x22, 0xC1};
		memcpy((void*)0x0043937C, &FuckYouLG, 6); // Trap :: It will crash main on send packet
	}

	hCheckThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RecvCheckThread, NULL, NULL, NULL);

	dwAntiHackTime = GetTickCount();
	PrintModules(GetCurrentProcessId());


}

BOOL APIENTRY DllMain (HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			CMiniDump::Begin();
			//DWORD OldProtect;
			//VirtualProtect((LPVOID)0x00401000, 13308926, PAGE_EXECUTE_READ, &OldProtect);
			//CheckMainFunctions();
			DoHooks();
			SetHook();
			SetValues();
			//g_Controller.m_Instance = hModule;
		}
		break;

		case DLL_PROCESS_DETACH:
			CMiniDump::End();
			break;
	}

	return TRUE;
}