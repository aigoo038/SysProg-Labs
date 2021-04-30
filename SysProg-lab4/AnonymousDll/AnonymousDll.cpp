// AnonymousDll.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "pch.h"
#include "framework.h"
#include "AnonymousDll.h"
#include <string>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CAnonymousDllApp, CWinApp)
END_MESSAGE_MAP()

CAnonymousDllApp::CAnonymousDllApp()
{	

}

CAnonymousDllApp theApp;

HANDLE hRead, hWrite;

void Cleanup()
{
	CloseHandle(hRead);
	CloseHandle(hWrite);
}

extern "C" _declspec(dllexport) void __stdcall ProcessClient()
{
	static int i = 0;
	while (true)
	{
		std::string s = GetString(hPipe);
		std::cout << s << std::endl;
		if (s == "quit")
		{
			break;
		}
		SendInt(hPipe, i++);
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);+

	}
}

extern "C" _declspec(dllexport) void __stdcall LauncClient()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	CreateProcess(NULL, (LPSTR)"lab2cpp.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

}



extern "C" _declspec(dllexport) int __stdcall KillChild()
{
	return 0;
}

BOOL CAnonymousDllApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}
