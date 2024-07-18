//Exports, etc... 
#pragma once
#include <vector>
#include <stdio.h>
#include <Windows.h>
#define UPLAY_EXPORT __declspec(dllexport)
#define UPLAY_CALL __cdecl
namespace UPLAYEMU {

	extern "C"
	{
		UPLAY_EXPORT int UPLAY_CALL UPLAY_Start(int a1, bool a2);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_Update();

		UPLAY_EXPORT int UPLAY_CALL UPLAY_GetNextEvent();

		UPLAY_EXPORT int UPLAY_CALL UPLAY_SAVE_ReleaseGameList(DWORD a1);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_Quit();

		UPLAY_EXPORT int UPLAY_CALL UPLAY_HasOverlappedOperationCompleted(DWORD a1);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_GetOverlappedOperationResult(DWORD a1, DWORD a2);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_PRODUCT_GetProductList(DWORD a1, DWORD a2, DWORD a3);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_PRODUCT_ReleaseProductList(DWORD a1);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_GetLastError();

		UPLAY_EXPORT int UPLAY_CALL UPLAY_WIN_ReleaseRewardList(DWORD a1);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_SAVE_GetSavegames(void* ListPTR, void* MyStruct);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_SAVE_Open(int Dp1, int Dp2, void* Buf1, void* Buf2);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_SAVE_Write(HANDLE FileHandle, SIZE_T DataToWrite, void* Buf1, void* Buf2);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_SAVE_SetName(DWORD a1, DWORD a2);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_SAVE_Close(DWORD a1);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_SAVE_Remove(DWORD a1, DWORD a2);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_SAVE_Read(HANDLE hFile, SIZE_T DataToRead, long Offset, void* Dt1, DWORD* BytesReaded, void* MyStruct);

		UPLAY_EXPORT int UPLAY_CALL UPLAY_ACH_EarnAchievement(DWORD a1, DWORD a2);

		UPLAY_EXPORT const char* UPLAY_CALL UPLAY_USER_GetNameUtf8();

		UPLAY_EXPORT const char* UPLAY_CALL UPLAY_USER_GetAccountIdUtf8();


	}
	struct FileRead
	{
	public:

		DWORD Addr1;
		DWORD Addr2;
		DWORD Addr3;
	};
	struct FileList
	{
		DWORD Num;
		void* BufferString;
		DWORD Pointer;
	};
	struct FileRef
	{
		HANDLE hFile;
		int NumFile;
		char NameOfFile[4024];
	};
	struct FileOpen
	{
		DWORD D1;
	};
	struct FileOpenTwo
	{
		DWORD Addr1;
		DWORD Addr2;
		DWORD Addr3;
	};
	static char Data[1024];
	const char* Dir = "E:\\";
	int WhichFile = 0;
	const char* SaveFile;
	void* TempBuffer = VirtualAlloc(NULL, 0x80000, MEM_COMMIT, PAGE_READWRITE);
	std::vector<UPLAYEMU::FileRef> FileVector;
} // EPLAYEMU