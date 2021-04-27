// AnonChild.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <atlstr.h>
#include <fstream>
int main()
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	const int MAXLEN = 1024;
	while (true)
	{
		DWORD dwRead;
		char buff[MAXLEN+1];
		if (!ReadFile(hIn, buff, MAXLEN, &dwRead, nullptr) || !dwRead)
			{break;}
		buff[min(MAXLEN, dwRead)] = 0;
		cout << "child: " << buff << endl;
		CString filename;
		filename.Format("..\\%d.txt", 2);
		std::ofstream f;
		f.open(filename, std::ios::app);
		std::string msg = buff;
		f << msg.c_str() << std::endl;
		f.close();
	}
}
