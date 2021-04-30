#include "pch.h"
#include "framework.h"
#include "lab2cpp.h"
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
   
    
    HANDLE hMutex = CreateMutex(NULL, FALSE, "YeahMutex");
    HANDLE hEventStop = CreateEvent(NULL, TRUE, FALSE, StopName.c_str());
    

    WaitForSingleObject(hMutex, INFINITE);
    std::cout << "\nThread started  " << toStr(id) << std::endl;
    ReleaseMutex(hMutex);

    HANDLE hEvents[] = {hEventStop};

    while (true) {
        switch (WaitForMultipleObjects(1, hEvents, FALSE, INFINITE) - WAIT_OBJECT_0)
        {
            case 0:
            {
                ResetEvent(hEventStop);
                WaitForSingleObject(hMutex, INFINITE);
                std::cout << "\nThread ended  " << toStr(id) << std::endl;
                ReleaseMutex(hMutex);
                return 0;
            }
            /*case 1:
            {

            }*/
        }
    }
    return 0;
}


void start() {
    int thread_index = 1;

    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    auto MessageName = "NewMessage" + toStr(thread_index);

    auto hMutex = CreateMutex(NULL, FALSE, "YeahMutex");
    auto hEventStart = CreateEvent(NULL, TRUE, FALSE, "EventStart");
    auto hEventStop = CreateEvent(NULL, TRUE, FALSE, "EventStop");
    auto evSubmit = CreateEvent(NULL, TRUE, FALSE, "ThreadSubmit");
    auto evMain = CreateEvent(NULL, TRUE, FALSE, "evMain");
    auto hFile = CreateEvent(NULL, TRUE, FALSE, MessageName.c_str());

    HANDLE hEvents[] = { hEventStart, hEventStop, evMain, hFile};

    while (true) {
        switch (WaitForMultipleObjects(4, hEvents, FALSE, INFINITE) - WAIT_OBJECT_0)
        {
        case 0:
        {
            ResetEvent(hEventStart);
            std::thread th(YeahThread, (LPVOID)thread_index++);
            th.detach();
            SetEvent(evSubmit);
            break;
        }
        case 1:
        {
            if (!(thread_index == 1))
            {
                HANDLE hlEventStop = CreateEvent(NULL, TRUE, FALSE, ("EventStop" + toStr(thread_index--)).c_str());
                SetEvent(hlEventStop);
                ResetEvent(hlEventStop);
                SetEvent(evSubmit);
            }
            else
            {
                SetEvent(evSubmit);
                return;
            }
            break;
        }
        case 2:
        {
            ResetEvent(evMain);
            HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
            const int MAXLEN = 1024;
            DWORD dwRead;
            char buff[MAXLEN + 1];
            if (!ReadFile(hIn, buff, MAXLEN, &dwRead, nullptr) || !dwRead)
                break;
            buff[min(MAXLEN, dwRead)] = 0;
            WaitForSingleObject(hMutex, INFINITE);
            std::cout << buff << std::endl;
            ReleaseMutex(hMutex);
            break;
        }
        case 3:
        {
            HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
            const int MAXLEN = 1024;
            DWORD dwRead;
            char buff[MAXLEN + 1];
            if (!ReadFile(hIn, buff, MAXLEN, &dwRead, nullptr) || !dwRead)
            {
                break;
            }
            buff[min(MAXLEN, dwRead)] = 0;
            CString filename;
            filename.Format("..\\%d.txt", id);
            std::ofstream f;
            f.open(filename, std::ios::app);
            std::string msg = buff;
            f << msg.c_str() << std::endl;
            f.close();
            ResetEvent(hFile);

            break;
        }
        }
    }
    while (true) {
        SetEvent(hEventStart);
        ResetEvent(hEventStart);
    }
    CloseHandle(hMutex);
    CloseHandle(hEventStart);
    CloseHandle(hEventStop);
    CloseHandle(evSubmit);
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