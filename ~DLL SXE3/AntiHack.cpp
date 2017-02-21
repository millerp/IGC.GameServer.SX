#include "stdafx.h"
#include "AntiHack.h"
#include "packetsend_hooks.h"
#include "offsets.h"
#include "protocol.h"

// Globals
LPBYTE _GetTickCount = (LPBYTE)GetProcedureAddress(GetModuleHandle("kernel32.dll"), "GetTickCount");
BYTE originalqueryperformancecounter[5] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };
HANDLE hCheckThread = NULL;
// functions
void CheckSH()
{
	Sleep(50);
}
void AHCheckGetTickHook()
{
	if (_GetTickCount != NULL)
	{
		if (_GetTickCount[0] == 0xE9 || _GetTickCount[0] == 0x90)
		{
			ExitProcess(0);
		}
	}
}

//004E134D   C745 98 AA000000 MOV DWORD PTR SS:[EBP-68],0AA
//004E1325   6A 70            PUSH 70



void AHCheckSwapBufferMod()
{
	BYTE Val1[1];
	BYTE Val2[1];
	DWORD OldProtect;

	
		DWORD AH_SWAP_BUFFER_OFFSET1 = 0x004E49AD + 3; // SXE3
	DWORD AH_SWAP_BUFFER_OFFSET2 = 0x004E4985 + 1; // SXE3

	VirtualProtect((LPVOID)(AH_SWAP_BUFFER_OFFSET1), 4, PAGE_EXECUTE_READWRITE, &OldProtect);
	VirtualProtect((LPVOID)(AH_SWAP_BUFFER_OFFSET2), 1, PAGE_EXECUTE_READWRITE, &OldProtect);

	ReadProcessMemory(GetCurrentProcess(), (LPVOID)AH_SWAP_BUFFER_OFFSET1, Val1, 1, 0);
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)AH_SWAP_BUFFER_OFFSET1, Val2, 1, 0);


	if (Val1[0] != Val2[0])
	{
		GCSendAntihackBreach(90000);
		int ofs = rand() % 0xFFFFFFFF;
		_asm
		{
			mov edx, ofs;
			jmp edx;
		}
	}

	if (Val1[0] != 0x28 && Val2[0] != 0x28)
	{
		GCSendAntihackBreach(90000);
		int ofs = rand() % 0xFFFFFFFF;
		_asm
		{
			mov edx, ofs;
			jmp edx;
		}
	}

	
}

HMODULE GetCurrentModule()
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetCurrentModule, &hModule);

	return hModule;
}

int PrintModules( DWORD processID )
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    unsigned int i;

    // Print the process identifier.

    printf( "\nProcess ID: %u\n", processID );

    // Get a handle to the process.

    hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                            PROCESS_VM_READ,
                            FALSE, processID );
    if (NULL == hProcess)
        return 1;

   // Get a list of all the modules in this process.

    if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
        {
            TCHAR szModName[MAX_PATH];

            // Get the full path to the module's file.

            if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
                                      sizeof(szModName) / sizeof(TCHAR)))
            {
                // Print the module name and handle value.

                _tprintf( TEXT("\t%s (0x%08X)\n"), szModName, hMods[i] );
            }
        }
    }
    
    // Release the handle to the process.

    CloseHandle( hProcess );

    return 0;
}