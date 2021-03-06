// KanturuUtil.cpp: implementation of the CKanturuUtil class.
//	GS-N	1.00.18	JPN	0x005845B0	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gamemain.h"
#include "Kanturu.h"
#include "KanturuUtil.h"
#include "KanturuBattleUserMng.h"
#include "TLog.h"
#include "winutil.h"
#include "TNotice.h"
#include "configread.h"

CKanturuUtil g_KanturuUtil; 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuUtil::CKanturuUtil()
{
	return;
}

CKanturuUtil::~CKanturuUtil()
{
	return;
}


struct PMSG_ANS_KANTURU_STATE_CHANGE
{
	PBMSG_HEAD2 h;	// C1:D1:03
	BYTE btState;	// 4
	BYTE btDetailState;	// 5
};

void CKanturuUtil::NotifyKanturuChangeState(int iState, int iDetailState)
{
	PMSG_ANS_KANTURU_STATE_CHANGE pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x03, sizeof(pMsg));
	pMsg.btState = iState;
	pMsg.btDetailState = iDetailState;

	this->SendDataKanturuBossMapUser((LPBYTE)&pMsg, sizeof(pMsg));
}


struct PMSG_ANS_ENTER_KANTURU_BOSS_MAP
{
	PBMSG_HEAD2 h;	// C1:D1:01
	BYTE btResult;	// 4
};


void CKanturuUtil::NotifyKanturuEntranceReqResult(int iIndex, int iResult)
{
	PMSG_ANS_ENTER_KANTURU_BOSS_MAP pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x01, sizeof(pMsg));
	pMsg.btResult = iResult;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}


struct PMSG_ANS_KANTURU_BATTLE_RESULT
{
	PBMSG_HEAD2 h;	// C1:D1:04
	BYTE btResult;	// 4
};



void CKanturuUtil::NotifyKanturuSuccessValue(int iSuccessValue)
{
	PMSG_ANS_KANTURU_BATTLE_RESULT pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x04, sizeof(pMsg));
	pMsg.btResult = iSuccessValue;

	this->SendDataKanturuBattleUser((LPBYTE)&pMsg, sizeof(pMsg));
}


struct PMSG_ANS_KANTURU_CURRENT_STATE
{
	PBMSG_HEAD2 h;	// C1:D1:02
	BYTE btCurrentState;	// 4
	BYTE btCurrentDetailState;	// 5
};


void CKanturuUtil::NotifyKanturuCurrentState(int iIndex, int iState, int iDetailState)
{
	PMSG_ANS_KANTURU_CURRENT_STATE pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x02, sizeof(pMsg));
	pMsg.btCurrentState = iState;
	pMsg.btCurrentDetailState = iDetailState;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}



struct PMSG_ANS_KANTURU_BATTLE_SCENE_TIMELIMIT
{
	PBMSG_HEAD2 h;	// C1:D1:05
	int btTimeLimit;	// 4
};


void CKanturuUtil::NotifyKanturuBattleTime(int iBattleTime)
{
	PMSG_ANS_KANTURU_BATTLE_SCENE_TIMELIMIT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x05, sizeof(pMsg));
	pMsg.btTimeLimit = iBattleTime;

	this->SendDataKanturuBattleUser((LPBYTE)&pMsg, sizeof(pMsg));
}


struct PMSG_NOTIFY_KANTURU_WIDE_AREA_ATTACK
{
	PBMSG_HEAD2 h;	// C1:D1:06
	BYTE btObjClassH;	// 4
	BYTE btObjClassL;	// 5
	BYTE btType;	// 6
};


void CKanturuUtil::NotifyKanturuWideAreaAttack(int iIndex, int iTargetIndex, int iSkillType)
{
	PMSG_NOTIFY_KANTURU_WIDE_AREA_ATTACK pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x06, sizeof(pMsg));
	pMsg.btObjClassH = SET_NUMBERH(gObj[iIndex].Class);
	pMsg.btObjClassL = SET_NUMBERL(gObj[iIndex].Class);
	pMsg.btType = iSkillType;

	this->SendDataToUser(iTargetIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}



struct PMSG_NOTIFY_KANTURU_USER_MONSTER_COUNT
{
	PBMSG_HEAD2 h;	// C1:D1:07
	BYTE btMonsterCount;	// 4
	BYTE btUserCount;	// 5
};


void CKanturuUtil::NotifyKanturuUserMonsterCount(int iMonsterCount, int iUserCount)
{
	PMSG_NOTIFY_KANTURU_USER_MONSTER_COUNT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x07, sizeof(pMsg));
	pMsg.btMonsterCount = iMonsterCount;
	pMsg.btUserCount = iUserCount;

	this->SendDataKanturuBattleUser((LPBYTE)&pMsg, sizeof(pMsg));
}

void CKanturuUtil::SendMsgKanturuBattleUser(LPSTR lpszMsg, ...)
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);

	int iUserIndex = 0;

	for ( int iCount=0;iCount<MAX_KANTURU_BATTLE_USER;iCount++)
	{
		iUserIndex = g_KanturuBattleUserMng.m_BattleUser[iCount].GetIndex();

		if ( iUserIndex != -1 && gObj[iUserIndex].Connected == PLAYER_PLAYING &&
			 gObj[iUserIndex].Type == OBJ_USER )
		{
			TNotice::SendNoticeToUser(iUserIndex, &pNotice);
		}
	}
}


void CKanturuUtil::SendDataKanturuBattleUser(LPBYTE lpMsg, int iSize)
{
	int iUserIndex=0;

	for ( int iCount=0;iCount<MAX_KANTURU_BATTLE_USER;iCount++)
	{
		iUserIndex = g_KanturuBattleUserMng.m_BattleUser[iCount].GetIndex();

		if ( iUserIndex != -1 && gObj[iUserIndex].Connected == PLAYER_PLAYING &&
			 gObj[iUserIndex].Type == OBJ_USER )
		{
			IOCP.DataSend(iUserIndex, lpMsg, iSize);
		}
	}
}



void CKanturuUtil::SendMsgKauturuBossMapUser(LPSTR lpszMsg, ...)
{
	if (!lpszMsg)
		return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);

	for (int iCount = g_ConfigRead.server.GetObjectStartUserIndex(); iCount < g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if (gObj[iCount].Connected == PLAYER_PLAYING &&
			gObj[iCount].Type == OBJ_USER &&
			gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS)
		{
			TNotice::SendNoticeToUser(iCount, &pNotice);
		}
	}
}



void CKanturuUtil::SendDataKanturuBossMapUser(LPBYTE lpMsg, int iSize)
{
	for (int iCount = g_ConfigRead.server.GetObjectStartUserIndex(); iCount < g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if (gObj[iCount].Connected == PLAYER_PLAYING &&
			gObj[iCount].Type == OBJ_USER &&
			gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS)
		{
			IOCP.DataSend(iCount, lpMsg, iSize);
		}
	}
}



void CKanturuUtil::SendMsgKauturuMapUser(LPSTR lpszMsg, ...)
{
	if (!lpszMsg)
		return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);

	for (int iCount = g_ConfigRead.server.GetObjectStartUserIndex(); iCount < g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if (gObj[iCount].Connected == PLAYER_PLAYING &&
			gObj[iCount].Type == OBJ_USER &&
			(gObj[iCount].MapNumber == MAP_INDEX_KANTURU1 ||
				gObj[iCount].MapNumber == MAP_INDEX_KANTURU2 ||
				gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS))
		{
			TNotice::SendNoticeToUser(iCount, &pNotice);
		}
	}
}


void CKanturuUtil::SendDataKanturuMapUser(LPBYTE lpMsg, int iSize)
{
	for (int iCount = g_ConfigRead.server.GetObjectStartUserIndex(); iCount < g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if (gObj[iCount].Connected == PLAYER_PLAYING &&
			gObj[iCount].Type == OBJ_USER &&
			(gObj[iCount].MapNumber == MAP_INDEX_KANTURU1 ||
				gObj[iCount].MapNumber == MAP_INDEX_KANTURU2 ||
				gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS))
		{
			IOCP.DataSend(iCount, lpMsg, iSize);
		}
	}
}



void CKanturuUtil::SendMsgAllUser(LPSTR lpszMsg, ...)
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);
	
	TNotice::SendNoticeToAllUser(&pNotice);
}




void CKanturuUtil::SendDataAllUser(LPBYTE lpMsg, int iSize)
{
	for (int iCount = g_ConfigRead.server.GetObjectStartUserIndex(); iCount < g_ConfigRead.server.GetObjectMax(); iCount++)
	{
		if (gObj[iCount].Connected == PLAYER_PLAYING &&
			gObj[iCount].Type == OBJ_USER)
		{
			IOCP.DataSend(iCount, lpMsg, iSize);
		}
	}
}


void CKanturuUtil::SendMsgToUser(int iIndex, LPSTR lpszMsg, ...)
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);
	
	TNotice::SendNoticeToUser(iIndex, &pNotice);
}


void CKanturuUtil::SendDataToUser(int iIndex, LPBYTE lpMsg, int iSize)
{
	if ( gObj[iIndex].Connected == PLAYER_PLAYING &&
		 gObj[iIndex].Type == OBJ_USER )
	{
		IOCP.DataSend(iIndex, lpMsg, iSize);
	}
}


// #error Bad Coded Function
#pragma warning ( disable : 4700 )
void CKanturuUtil::SendKanturuChattingMsg(int iIndex, LPSTR lpMsg, ...)
{
	return;
	LPOBJ lpObj;
	char szChat[512];

	for (int iCount = 0; iCount < MaxViewportMonster; iCount++)
	{
		if ( lpObj->VpPlayer2[iCount].state )
		{
			int number = lpObj->VpPlayer2[iCount].number;

			if ( number >= 0 || number < g_ConfigRead.server.GetObjectMax() )
			{
				GSProtocol.ChatTargetSend(lpObj, szChat, number);
			}
		}
	}
}
#pragma warning ( default : 4700 )



struct PMSG_REQ_LOG_KANTURU_TIMEATTACK_EVENT
{
	PBMSG_HEAD2 h;	// C1:22:00
	short nINDEX;	// 4
	char szUID[11];	// 6
	char szNAME[11];	// 11
	unsigned short wServerCode;	// 1C
	char szBattleID[14];	// 1E
	BYTE btStageNumber;	// 2C
	WORD wClearTime;	// 2E
	int iLevel;	// 30
	int iExp;	// 34
};


void CKanturuUtil::SendDataKanturuTimeAttackEvent(int iIndex, BYTE btFlag, int iClearTime)
{
	PMSG_REQ_LOG_KANTURU_TIMEATTACK_EVENT pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBE, 0x22, sizeof(pMsg));
	pMsg.nINDEX = iIndex;
	memcpy(pMsg.szUID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN);
	pMsg.szUID[10] = '\0';	// #error Change 11 to 10
	memcpy(pMsg.szNAME, gObj[iIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.szNAME[10] = '\0';	// #error Change 11 to 10
	pMsg.wServerCode = g_ConfigRead.server.GetGameServerCode() / 20;	// #warning Change the 20 for a posible macro of MapServerInfo

	char szKanturuBattleDate[14];
	memset(szKanturuBattleDate, 0, sizeof(szKanturuBattleDate));

	wsprintf(szKanturuBattleDate, "%d%03d%02d",
		g_Kanturu.GetKanturuBattleDate(), g_ConfigRead.server.GetGameServerCode(),
		g_Kanturu.GetKanturuBattleCounter());
	
	memcpy(pMsg.szBattleID, szKanturuBattleDate, sizeof(pMsg.szBattleID));
	pMsg.btStageNumber = btFlag;
	pMsg.wClearTime = iClearTime;
	pMsg.iLevel = gObj[iIndex].Level;
	pMsg.iExp = gObj[iIndex].m_PlayerData->Experience;

	g_Log.Add("[KANTURU][TimeAttackEvent] [%s][%s] %d/BattleID:%s, StageNum:%d, ClearTime:%d, Level:%d(%d)",
		pMsg.szUID, pMsg.szNAME, pMsg.wServerCode, pMsg.szBattleID, pMsg.btStageNumber,
		pMsg.wClearTime, pMsg.iLevel, pMsg.iExp);

	wsDataCli.DataSend((PCHAR)&pMsg, sizeof(pMsg));
}
