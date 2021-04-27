// AnonymousDll.h: основной файл заголовка для библиотеки DLL AnonymousDll
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


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
