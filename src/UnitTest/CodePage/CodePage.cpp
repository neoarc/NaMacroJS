#include "stdafx.h"

#include <iostream>

int main()
{
	_wsetlocale(LC_ALL, L"korean");
	//setlocale(LC_ALL, "ko_kr.utf8");

	// 한글 출력이 왜 안되지?
	wprintf(_T("%ls"), (const wchar_t*)L"test ㄱㄴㄷ\n");

	std::cout << "Press any number and enter to exit.";
	int temp;
	std::cin >> temp;

    return 0;
}

