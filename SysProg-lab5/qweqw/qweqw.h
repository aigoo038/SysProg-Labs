// qweqw.h: основной файл заголовка для библиотеки DLL qweqw
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CqweqwApp
// Реализацию этого класса см. в файле qweqw.cpp
//

class CqweqwApp : public CWinApp
{
public:
	CqweqwApp();

// Переопределение
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
