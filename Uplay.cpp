#include "Uplay.hpp"
static const char* AttachDirFile(const char* Path, const char* File)
{
	memset(UPLAYEMU::Data, 0, sizeof(UPLAYEMU::Data));
	sprintf_s(UPLAYEMU::Data, sizeof(UPLAYEMU::Data), "%s\\%s", Path, File);
	return UPLAYEMU::Data;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_Start(int a1, bool a2)
{
	return 0;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_Update()
{
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_GetNextEvent()
{
	return 0;
}


UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_SAVE_ReleaseGameList(DWORD a1)
{
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_Quit()
{
	return 0;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_HasOverlappedOperationCompleted(DWORD a1)
{
	return 0x2913001;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_GetOverlappedOperationResult(DWORD a1, DWORD a2)
{
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_PRODUCT_GetProductList(DWORD a1, DWORD a2, DWORD a3)
{
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_PRODUCT_ReleaseProductList(DWORD a1)
{
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_GetLastError()
{
	return 0;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_WIN_ReleaseRewardList(DWORD a1)
{
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_SAVE_GetSavegames(void* ListPTR, void* MyStruct)
{
	if (!MyStruct)
		return 0;

	void* FirstPTR = VirtualAlloc(nullptr, 0x80000, MEM_COMMIT, PAGE_READWRITE);
	void* SecondPTR = VirtualAlloc(nullptr, 0x80000, MEM_COMMIT, PAGE_READWRITE);
	ULONG_PTR SecondPTRAddress = reinterpret_cast<ULONG_PTR>(SecondPTR);
	const char* Constant = AttachDirFile(UPLAYEMU::Dir, "*.save");
	ULONG_PTR VOF{ 0 };
	WIN32_FIND_DATAA FD = { 0 };
	HANDLE FirstFile = FindFirstFileA(Constant, &FD);
	if (FirstFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (FD.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
			{
				void* ThirdPTR = VirtualAlloc(NULL, 0x3000, MEM_COMMIT, PAGE_READWRITE);
				int FileCtr{ 0 };
				char Bytes[270] = { 0 };
				strcpy(Bytes, (char*)FD.cFileName);
				int Size = strlen(Bytes);
				for (int i = Size; i > 0; --i)
				{
					if (Bytes[i] == '.')
					{
						memset(&Bytes[i],0 ,Size - i);
						FileCtr = strtoull(Bytes, NULL, 16);
						break;
					}
				}
				HANDLE hFoundFile = CreateFileA(AttachDirFile(Dir, (char*)FD.cFileName), GENERIC_READ, 1, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				DWORD FileSize = GetFileSize(hFoundFile, NULL);
				DWORD  Val = FileSize - 0x228;
				DWORD FilePointer = SetFilePointer(hFoundFile, 0x28, NULL, NULL);
				DWORD BytesReaded{ 0 };
				char* Datar2 = (char*)VirtualAlloc(NULL, 0x200, MEM_COMMIT, PAGE_READWRITE);
				ReadFile(hFoundFile, Datar2, 0x200, &BytesReaded, NULL);
				UPLAYEMU::FileList* FileL = reinterpret_cast<UPLAYEMU::FileList*>(ThirdPTR);
				FileL->BufferString = Datar2;
				FileL->Num = (DWORD)FileCtr;
				FileL->Pointer = Val;
#ifdef _WIN64
				memcpy((void*)SecondPTRAddress, &ThirdPTR, 8);
				SecondPTRAddress += 8;
#else
				memcpy((void*)SecondPTRAddress, &ThirdPTR, 4);
				SecondPTRAddress += 4;
#endif
				CloseHandle(hFoundFile);
				VOF++;
			}

		} while (FindNextFileA(FirstFile, &FD) == TRUE);
		FindClose(FirstFile);
#ifdef _WIN64
		memcpy(FirstPTR, &VOF, 8);
		memcpy((void*)((ULONG_PTR)FirstPTR + 8), &SecondPTR, 8);
#else
		memcpy(FirstPTR, &VOF, 4);
		memcpy((void*)((ULONG_PTR)FirstPTR + 4), &SecondPTR, 4);
#endif
	}
#ifdef _WIN64 
	memcpy(ListPTR, &FirstPTR, 8);
#else
	memcpy(ListPTR, &FirstPTR, 4);
#endif
	UPLAYEMU::FileRead* FileR = reinterpret_cast<UPLAYEMU::FileRead*>(MyStruct);
	FileR->Addr1++;
	FileR->Addr2 = 1;
	FileR->Addr3 = 0;
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_SAVE_Open(int Dp1, int Dp2, void* Buf1, void* Buf2)
{
	char File[1024] = { 0 };
	sprintf(File, "%x.save", Dp1);
	UPLAYEMU::WhichFile = Dp1;
	UPLAYEMU::SaveFile = AttachDirFile(UPLAYEMU::Dir, File);
	UPLAYEMU::TempBuffer = VirtualAlloc(NULL, 0x228, MEM_COMMIT, PAGE_READWRITE);
	UPLAYEMU::FileRef Ref = { 0 };
	if (Dp2 == 0)
	{
		UPLAYEMU::FileOpen* FP = reinterpret_cast<UPLAYEMU::FileOpen*>(Buf1);
		UPLAYEMU::FileOpenTwo* FP2 = reinterpret_cast<UPLAYEMU::FileOpenTwo*>(Buf2);
		HANDLE FileUplayt = CreateFileA(SaveFile, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (FileUplayt == INVALID_HANDLE_VALUE)
		{
			FP2->Addr1++;
			FP2->Addr2 = 1;
			FP2->Addr3 = 0;
			return 0;
		}
		FP->D1 = reinterpret_cast<DWORD>(FileUplayt);
		FP2->Addr1++;
		FP2->Addr2 = 1;
		FP2->Addr3 = 0;
		Ref.hFile = FileUplayt;
		Ref.NumFile = Dp1;
		FileVector.push_back(Ref);
		return 1;
	}
	else
	{
		UPLAYEMU::FileOpen* FP = reinterpret_cast<UPLAYEMU::FileOpen*>(Buf1);
		UPLAYEMU::FileOpenTwo* FP2 = reinterpret_cast<UPLAYEMU::FileOpenTwo*>(Buf2);
		HANDLE FileUplayt = CreateFileA(SaveFile, SE_GROUP_LOGON_ID, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (FileUplayt == INVALID_HANDLE_VALUE)
		{
			FP2->Addr1++;
			FP2->Addr2 = 1;
			FP2->Addr3 = 0;
			return 0;
		}
		DWORD Ld;
		WriteFile(FileUplayt, TempBuffer, 0x228, &Ld, 0);
		FP->D1 = reinterpret_cast<DWORD>(FileUplayt);
		FP2->Addr1++;
		FP2->Addr2 = 1;
		FP2->Addr3 = 0;
		Ref.hFile = FileUplayt;
		Ref.NumFile = Dp1;
		FileVector.push_back(Ref);
		return 1;
	}
	return 0;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_SAVE_Write(HANDLE FileHandle, SIZE_T DataToWrite, void* Buf1, void* Buf2)
{
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		void* WriteData = nullptr;
#ifdef _WIN64
		memcpy(&WriteData, Buf1, 8);
#else
		memcpy(&WriteData, Buf1, 4);
#endif //_WIN64

		DWORD nNumberOfBytesToWrite{ 0 };
		UPLAYEMU::FileRead* FileStruct = reinterpret_cast<UPLAYEMU::FileRead*>(Buf2);
		if (!WriteFile(FileHandle, WriteData, DataToWrite, &nNumberOfBytesToWrite, nullptr))
		{
			FileStruct->Addr1++;
			FileStruct->Addr2 = 1;
			FileStruct->Addr3 = 0;
			return 0;
		}
		FileStruct->Addr1++;
		FileStruct->Addr2 = 1;
		FileStruct->Addr3 = 0;
		return 1;
	}
	UPLAYEMU::FileRead* FileStruct = reinterpret_cast<UPLAYEMU::FileRead*>(Buf2);
	FileStruct->Addr1++;
	FileStruct->Addr2 = 1;
	FileStruct->Addr3 = 0;
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_SAVE_SetName(DWORD a1, DWORD a2)
{
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_SAVE_Close(DWORD a1)
{
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_SAVE_Remove(DWORD a1, DWORD a2)
{
	return 1;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_SAVE_Read(HANDLE hFile, SIZE_T DataToRead, long Offset, void* Dt1, DWORD* BytesReaded, void* MyStruct)
{
	if (!MyStruct)
		return 0;

	void* ReadedData = nullptr;
	if (hFile != INVALID_HANDLE_VALUE)
	{
#ifdef _WIN64
		memcpy(&ReadedData, Dt1, 8);
#else
		memcpy(&ReadedData, Dt1, 4);
#endif
		SetFilePointer(hFile, 0x228 + Offset, NULL, NULL);
		DWORD Wrote;
		if (!ReadFile(hFile, ReadedData, DataToRead, BytesReaded, NULL))
		{
			UPLAYEMU::FileRead* FileR = reinterpret_cast<UPLAYEMU::FileRead*>(MyStruct);
			FileR->Addr1++;
			FileR->Addr2 = 1;
			FileR->Addr3 = 0;
			return 0;
		}
		UPLAYEMU::FileRead* FileR = reinterpret_cast<UPLAYEMU::FileRead*>(MyStruct);
		FileR->Addr1++;
		FileR->Addr2 = 1;
		FileR->Addr3 = 0;
		return 1;
	}
	UPLAYEMU::FileRead* FileR = reinterpret_cast<UPLAYEMU::FileRead*>(MyStruct);
	FileR->Addr1++;
	FileR->Addr2 = 1;
	FileR->Addr3 = 0;
	return 0;
}

UPLAY_EXPORT int UPLAY_CALL UPLAYEMU::UPLAY_ACH_EarnAchievement(DWORD a1, DWORD a2)
{
	return 1;
}


UPLAY_EXPORT const char* UPLAY_CALL UPLAYEMU::UPLAY_USER_GetNameUtf8()
{
	return "Payn0h";
}

UPLAY_EXPORT const char* UPLAY_CALL UPLAYEMU::UPLAY_USER_GetAccountIdUtf8()
{
	return "c927c91e-0000-40c1-0000-5a5431c6b12e"; // Random account id 

}



