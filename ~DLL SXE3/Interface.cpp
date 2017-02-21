#include "stdafx.h"
#include "Interface.h"
#include "Text.h"
#include "Macros.h"
#include "offsets.h"

void __declspec(naked) MoveWndFix()
{
	__asm {
		PUSH 4;
		PUSH EBX;
		ADD DWORD PTR SS:[ESP], 8;
		CALL DWORD PTR DS:[IsBadWritePtr];
		CMP EAX, 0;
		JE ValidPtr;
		MOV EDX, 0x00CDF861; // SXE3
		JMP EDX;
ValidPtr:
		ADD DWORD PTR DS:[EBX+8],-1;
		JNZ lb1;
		MOV EDX, 0x00CDF851; // SXE3
		JMP EDX;
lb1:
		MOV EDX, 0x00CDF858; // SXE3
		JMP EDX;
	}
}