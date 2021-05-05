#include "pch.h"
#include "framework.h"
#include "lab4cpp.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PIPE_NAME (LPCSTR)"\\\\.\\pipe\\YeahPipe"
CWinApp theApp;

using namespace std;


std::string toStr(int i)
{
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    return s;
}


UINT __cdecl YeahThread(HANDLE NamedPipe, LPVOID lpParameter)
{
    int id = int(lpParameter);
    auto StopName = "EventStop" + toStr(id);

    HANDLE hMutex = CreateMutex(NULL, FALSE, "YeahMutex");

    WaitForSingleObject(hMutex, INFINITE);
    std::cout << "\nThread started  " << toStr(id) << std::endl;
    ReleaseMutex(hMutex);


        int s = -1;
        DWORD dwRead;
        ReadFile(NamedPipe, LPVOID(&s), sizeof(s), &dwRead, NULL);
        cout << s << endl;
        if (s == 1)
        {
            WaitForSingleObject(hMutex, INFINITE);
            std::cout << "\nThread stopped  " << toStr(id) << std::endl;
            ReleaseMutex(hMutex);
            return 0;
        }

    DisconnectNamedPipe(NamedPipe);
    CloseHandle(NamedPipe);
    return 0;
}


void start() {
    int thread_index = 1;

    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

    while (true)
    {
        HANDLE hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 1024, 1024, 0, NULL);
        DWORD dwRead;
        int id_event = 1;
        ConnectNamedPipe(hPipe, NULL);
        ReadFile(hPipe, LPVOID(&id_event), sizeof(id_event), &dwRead, NULL);
        switch (id_event)
        {
        case 0:
        {
            thread t(YeahThread, &hPipe, (LPVOID)thread_index++);
            t.detach();
            break;
        }

        case 1:
        {
            // Stop thread
            if (!(thread_index == 1))
            {
                auto StopName = "EventStop" + toStr(thread_index--);
                HANDLE hlEventStop = CreateEvent(NULL, TRUE, FALSE, StopName.c_str());
                SetEvent(hlEventStop);
                ResetEvent(hlEventStop);
            }
            else
            {
                return;
            }

            break;
        }

        case 2:
        {

            DWORD dwDone;
            int nLength = GetInt(hPipe);

            vector <char> v(nLength);
            ReadFile(hPipe, &v[0], nLength, &dwDone, NULL);

            vector <char> msg(GetInt(hPipe));
            // Read mesage
            int size = 0;
            ReadFile(hPipe, LPVOID(&size), sizeof(size), &dwRead, NULL);
            msg.push_back(size);
            ReadFile(hPipe, &msg, size, &dwRead, NULL);
            FlushFileBuffers(hPipe);
            DisconnectNamedPipe(hPipe);
        }
        }
    }
    // DisconnectNamedPipe(hPipe);
    // CloseHandle(hPipe);
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