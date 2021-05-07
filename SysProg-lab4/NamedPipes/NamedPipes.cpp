// AnonymousDll.cpp: определяет процедуры инициализации для библиотеки DLL.
//
#include "pch.h"
#include "framework.h"
#include "NamedPipes.h"
#include <string>
#include "..\\lab4cpp\\lab4cpp.h"

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


	

extern "C" 
{
	_declspec(dllexport) void __stdcall PassTo(int evType, int thId = 0, char* msg = 0)
	{
		switch (evType)
		{
			case 0:
			{
				if (WaitNamedPipe(PIPE_NAME, 5000))
				{

					HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
					DWORD dwRead, dwWrite;
					WriteFile(hPipe, LPCVOID(&evType), sizeof(evType), &dwWrite, NULL);
					CloseHandle(hPipe);
				}
				break;
			}
			case 1:
			{
				if (WaitNamedPipe(PIPE_NAME, 5000))
				{
					HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
					DWORD dwRead, dwWrite;

					WriteFile(hPipe, LPCVOID(&evType), sizeof(evType), &dwWrite, NULL);

					CloseHandle(hPipe);
				}
				break;
			}
			case 2:
			{
				if (WaitNamedPipe(PIPE_NAME, 5000))
				{
					int strSize = strlen(msg) + 1;
					HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
					DWORD dwRead, dwWrite;
					WriteFile(hPipe, LPCVOID(&evType), sizeof(evType), &dwWrite, NULL);
					WriteFile(hPipe, LPCVOID(&thId), sizeof(thId), &dwWrite, NULL);
					WriteFile(hPipe, LPCVOID(&strSize), sizeof(strSize), &dwWrite, NULL);
					WriteFile(hPipe, LPCVOID(msg), sizeof(strSize), &dwWrite, NULL);
					CloseHandle(hPipe);
				}
				break;
			}
			case 3:
			{
				STARTUPINFO si = { sizeof(si) };
				PROCESS_INFORMATION pi;
				CreateProcess(NULL, (LPSTR)"..\\..\\Debug\\lab4cpp.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			}
		}
	}

	_declspec(dllexport) void __stdcall StartThread(int evType)
	{
		
			  if (WaitNamedPipe(PIPE_NAME, 5000))
			  {

				  HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
				  DWORD dwRead, dwWrite;
				  WriteFile(hPipe, LPCVOID(&evType), sizeof(evType), &dwWrite, NULL);
				  CloseHandle(hPipe);
			  }
		
		
	}

	_declspec(dllexport) void __stdcall StartServer(int evType)
	{
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		CreateProcess(NULL, (LPSTR)"..\\..\\Debug\\lab4cpp.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}


	_declspec(dllexport) void __stdcall StopThread(int evType)
	{
		if (WaitNamedPipe(PIPE_NAME, 5000))
		{
			HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			DWORD dwRead, dwWrite;

			WriteFile(hPipe, LPCVOID(&evType), sizeof(evType), &dwWrite, NULL);

			CloseHandle(hPipe);
		}
	}


	 _declspec(dllexport) void __stdcall Send(int evType, int thId, const char* msg) 
	 {
		if (WaitNamedPipe(PIPE_NAME, 5000))
		{
			int strSize = strlen(msg) + 1;
			HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			DWORD dwRead, dwWrite;			
			WriteFile(hPipe, LPCVOID(&evType), sizeof(evType), &dwWrite, NULL);
			WriteFile(hPipe, LPCVOID(&thId), sizeof(thId), &dwWrite, NULL); 
			//WriteFile(hPipe, LPCVOID(&strSize), sizeof(strSize), &dwWrite, NULL);
			WriteFile(hPipe, LPCVOID(msg), strlen(msg), &dwWrite, NULL);
			CloseHandle(hPipe);
		}
	}
}

BOOL CNamedPipesApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}
