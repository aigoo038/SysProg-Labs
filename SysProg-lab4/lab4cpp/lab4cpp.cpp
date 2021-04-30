
#include "pch.h"
#include "framework.h"
#include "lab4cpp.h"
#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <fstream>
#include <thread>
#include <vector>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

using namespace std;

struct Message 
{ 
    int index; 
    char data[]; 
};

std::string toStr(int i)
{
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    return s;
}


UINT __cdecl YeahThread(LPVOID lpParameter)
{
    int id = int(lpParameter);
    auto StopName = "EventStop" + toStr(id);
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!


    return 0;
}


void start() {
    int thread_index = 1;

    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    thread th(YeahThread, (LPVOID)thread_index++);
    th.detach();
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!
    // CHANGE!

}

void LaunchClient()
{
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    CreateProcess(NULL, (LPSTR)"PipesClient.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}


void ProcessClient(HANDLE hPipe)
{
    static int i = 0;
    while (true)
    {
        string s = GetString(hPipe);
        cout << s << endl;
        if (s == "quit")
        {
            break;
        }
        SendInt(hPipe, ++i);
    }
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
}


int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            wprintf(L"Критическая ошибка: сбой при инициализации MFC\n");
            nRetCode = 1;
        }
        else
        {
            start();
        }
    }
    else
    {
        wprintf(L"Критическая ошибка: сбой GetModuleHandle\n");
        nRetCode = 1;
    }

    return nRetCode;
}