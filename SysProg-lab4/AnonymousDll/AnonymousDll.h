// AnonymousDll.h: основной файл заголовка для библиотеки DLL AnonymousDll
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы
#include <string>
#include <vector>

using namespace std;

inline int GetInt(HANDLE hPipe)
{
	DWORD dwDone;
	int n;

	ReadFile(hPipe, &n, sizeof(n), &dwDone, NULL);
	return n;
}

inline void SendInt(HANDLE hPipe, int n, bool bFlush = true)
{
	DWORD dwDone;
	WriteFile(hPipe, &n, sizeof(n), &dwDone, NULL);
	if (bFlush)
		FlushFileBuffers(hPipe);
}

inline string GetString(HANDLE hPipe)
{
	DWORD dwDone;
	int nLength = GetInt(hPipe);

	vector <char> v(nLength);
	ReadFile(hPipe, &v[0], nLength, &dwDone, NULL);
	return string(&v[0], nLength);
}

inline void SendString(HANDLE hPipe, const string& s)
{
	DWORD dwDone;
	int nLength = s.length();

	SendInt(hPipe, nLength, false);
	WriteFile(hPipe, s.c_str(), nLength, &dwDone, NULL);
	FlushFileBuffers(hPipe);
}


// CAnonymousDllApp
// Реализацию этого класса см. в файле AnonymousDll.cpp
//

class CAnonymousDllApp : public CWinApp
{
public:
	CAnonymousDllApp();

// Переопределение
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
