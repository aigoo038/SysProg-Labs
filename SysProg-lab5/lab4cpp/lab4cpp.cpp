#include "pch.h"
#include "framework.h"
#include "lab4cpp.h"
#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CWinApp theApp;

using namespace std;


DWORD WINAPI YeahThread(LPVOID lpParameter)//, SOCKET hSock)
{
    //CSocket s;
    //s.Attach(hSock);
    int id = int(lpParameter);
    HANDLE hEventStop = CreateEvent(NULL, TRUE, FALSE, "EventStop");
    HANDLE hMutex = CreateMutex(NULL, FALSE, "YeahMutex");
  
    HANDLE hEvents[] = { hEventStop };

    WaitForSingleObject(hMutex, INFINITE);
    std::cout << "\nThread started  " << InttoStr(id) << std::endl;
    ReleaseMutex(hMutex);
    while (true)
    {
        DWORD dwResult = WaitForMultipleObjects(1, hEvents, FALSE, INFINITE) - WAIT_OBJECT_0;
        
        switch (dwResult)
        {
            case 0:
            {
           
                ResetEvent(hEventStop);
                WaitForSingleObject(hMutex, INFINITE);
                std::cout << "\nThread stopped  " << std::endl; 
                ReleaseMutex(hMutex);
                stop = false;
                return 0;
                break;
            }
            
        }
    }
    return 0;
}


void start() 
{
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    HANDLE hMutex = CreateMutex(NULL, FALSE, "YeahMutex");
    HANDLE evStop = CreateEvent(NULL, TRUE, FALSE, "EventStop");
    int thread_index = 1;
    AfxSocketInit();
    CSocket Server;
    Server.Create(12345);
    Server.Listen();

    cout << Server << endl;
    while (1)
    {
        CSocket s; 
        Server.Accept(s);
        int evType = -1;
        s.Receive(LPVOID(&evType), sizeof(evType));
        switch (evType)
        {
            case 0:
            {
                //CreateThread(NULL, FALSE, YeahThread, (LPVOID)thread_index++, 0, NULL);
                thread t(YeahThread, (LPVOID)thread_index++);//, s.Detach());
                t.detach();
                break;
            }

            case 1:
            {
                if (!(thread_index == 1))
                {
                    stop = true;
                    SetEvent(evStop);
                    thread_index--;
                    ResetEvent(evStop);
                    break;
                }
                else
                {
                    return;
                }
                break;
            }

            case 2:
            {
                const int MAXLEN = 1024;
                int thId = 0;
                DWORD msgRead;
                char buff[MAXLEN + 1];
                s.Receive(LPVOID(&thId), sizeof(thId));
                s.Receive(LPVOID(&buff), sizeof(MAXLEN));
                buff[MAXLEN] = 0;//[min(MAXLEN, msgRead)] = 0;
                string msg(buff);  
                if (!thId == 0)
                {
                    WriteToFile(thId, msg.c_str());
                }
                else
                {
                    WaitForSingleObject(hMutex, INFINITE);
                    std::cout << "\nReceived Message  " << msg << " from:" << thId << std::endl;
                    ReleaseMutex(hMutex);
                }
                break;
            }
            CloseHandle(evStop);
        }
       
        
    }
    
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