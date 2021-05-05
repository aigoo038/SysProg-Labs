// AnonymousDll.cpp: определяет процедуры инициализации для библиотеки DLL.
//
#include "pch.h"
#include "framework.h"
#include "NamedPipes.h"

#define PIPE_NAME (LPCSTR)"\\\\.\\pipe\\YeahPipe"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CNamedPipesApp, CWinApp)
END_MESSAGE_MAP()

CNamedPipesApp::CNamedPipesApp()
{
}


CNamedPipesApp theApp;


extern "C" _declspec(dllexport) void __stdcall StartThread(int eventType)
{
	if (WaitNamedPipe(PIPE_NAME, 5000))
	{

		HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		DWORD dwRead, dwWrite;
		WriteFile(hPipe, LPCVOID(&eventType), sizeof(eventType), &dwWrite, NULL);
		//CloseHandle(hPipe);
	}

}
	

extern "C" 
{
	_declspec(dllexport) void __stdcall StartServer(int eventType)
	{
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		CreateProcess(NULL, (LPSTR)"..\\..\\Debug\\lab4cpp.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}


	_declspec(dllexport) void __stdcall StopThread(int eventType)
	{
		if (WaitNamedPipe(PIPE_NAME, 5000))
		{
			HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			DWORD dwRead, dwWrite;

			WriteFile(hPipe, LPCVOID(&eventType), sizeof(eventType), &dwWrite, NULL);

			CloseHandle(hPipe);
		}
	}


	 _declspec(dllexport) void __stdcall Send(int eventType, char* msg) {
		if (WaitNamedPipe(PIPE_NAME, 5000))
		{
			HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			DWORD dwRead, dwWrite;
			DWORD dwWrite;
			WriteFile(hPipe, LPCVOID(msg), sizeof(strlen(msg) + 1), &dwWrite, NULL);
			CloseHandle(hPipe);
		}
	}
}

BOOL CNamedPipesApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}
