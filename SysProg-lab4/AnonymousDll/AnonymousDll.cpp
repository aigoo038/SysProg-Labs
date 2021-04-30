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



// CHANGE!
// CHANGE!
// CHANGE!
// CHANGE!
// CHANGE!



BOOL CAnonymousDllApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}
