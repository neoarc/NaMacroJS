// NaHttpTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "..\..\NaLib\NaHttp.h"

int main()
{
	NaHttp http;
	//NaString strUrl(L"http://api.openweathermap.org/data/2.5/weather?lat=37.491412&lon=126.9806151&APPID=ebf52496ad9749992d249b5ba5f8eaa7");
	//NaString strUrl(L"www.google.com");
	NaString strUrl(L"www.daum.net");
	//NaString strUrl(L"www.naver.com");

	http.Open(strUrl);
	http.SendRequest();
	
	NaString strRes = http.GetResponse();
	printf("%s", strRes.cstr());

	::MessageBox(NULL, strRes.wstr(), L"", MB_OK);
	/*
	std::string ck = std::string("qaOgNbwSlaBfZljuFbsL1TBPN");
	std::string cs = std::string("2KIzuD82ASp9IGvbSUYqCV2CeNgVtMqd4cln5VMTygrte3QhpF");
	std::string tk = std::string("713937644-atFLOQ6fFCphm9H2Tqaf29w2jfKCcnpfwSanjEd6");
	std::string ts = std::string("WJyeuClQrv9cwqtD6eP05K8Nsj0aSQXcxPU2lIosSigm4");
	*/

    return 0;
}

