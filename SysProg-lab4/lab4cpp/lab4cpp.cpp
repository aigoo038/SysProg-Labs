#include "pch.h"
#include "framework.h"
#include "lab4cpp.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PIPE_NAME (LPCSTR)"\\\\.\\pipe\\YeahPipe"
CWinApp theApp;

using namespace std;


string toStr(int i)
{
    std::string s;
    std::stringstream out;
    out << i;
    s = out.str();
    return s;
}

void WriteFile(int index, string data)
{
    CString filename;
    filename.Format("..\\%d.txt", index);
    std::ofstream f;
    f.open(filename, std::ios::app);
    f << data.c_str() << std::endl;
    f.close();
    std::cout << "\nThread " + toStr(index) + " created file " << std::endl;
}

string thredToStop = "";


UINT __cdecl YeahThread(HANDLE NamedPipe, LPVOID lpParameter)
{
    int id = int(lpParameter);

    auto myid = this_thread::get_id();
    stringstream ss;
    ss << myid;
    thredToStop = ss.str();

    auto StopName = "EventStop" + toStr(id);

    HANDLE hMutex = CreateMutex(NULL, FALSE, "YeahMutex");
    HANDLE hEventStop = CreateEvent(NULL, TRUE, FALSE, StopName.c_str());

    WaitForSingleObject(hMutex, INFINITE);
    std::cout << "\nThread started  " << toStr(id) << std::endl;
    ReleaseMutex(hMutex);
    HANDLE hEvents[] = { hEventStop };
    return 0;
}


void start() {
    int thread_index = 1;

    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

    while (true)
    {
        HANDLE hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 1024, 1024, 0, NULL);
        HANDLE hMutex = CreateMutex(NULL, FALSE, "YeahMutex");

        DWORD dwRead, dwWrite;
        int evType = NULL;
        ConnectNamedPipe(hPipe, NULL);
        auto s = "";
        ReadFile(hPipe, LPVOID(&evType), sizeof(evType), &dwRead, NULL);
        switch (evType)
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
  
                
             
            }
            else
            {
                return;
            }

            break;
        }

        case 2:
        {
            int thId, msgSize = NULL;
            DWORD msgRead;
            char* msg;
            msg = new char[msgSize];
            

            ReadFile(hPipe, LPVOID(&thId), sizeof(thId), &msgRead, NULL);
            ReadFile(hPipe, LPVOID(&msgSize), sizeof(msgSize), &msgRead, NULL);
            ReadFile(hPipe, LPVOID(msg), sizeof(msgSize), &msgRead, NULL);
            FlushFileBuffers(hPipe);
            DisconnectNamedPipe(hPipe);
            std::string myString(msg, msgSize);
            if (!thId == 0)
            {
                WriteFile(thId, myString);
                DisconnectNamedPipe(hPipe);
                CloseHandle(hPipe);
            }
            else
            {
                WaitForSingleObject(hMutex, INFINITE);
                std::cout << "\nReceived Message  " << myString << "from:" << thId << std::endl;
                ReleaseMutex(hMutex);  
                DisconnectNamedPipe(hPipe);
                CloseHandle(hPipe);
            }

            break;
        }

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