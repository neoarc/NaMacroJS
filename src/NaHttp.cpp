#include "stdafx.h"
#include "NaHttp.h"


#include <Ws2tcpip.h>

NaHttp::NaHttp()
{
	m_bInit = false;
	m_bConnected = false;
	m_ConnectSocket = INVALID_SOCKET;
	m_enReqMethod = NA_HTTP_GET;
}

NaHttp::~NaHttp()
{
	WSACleanup();
}

bool NaHttp::Init()
{
	if (m_bInit)
		return true;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		// TODO error
		printf("WSAStartup failed\n");
		return false;
	}

	return m_bInit = true;
}

void NaHttp::Open(const wchar_t * addr)
{
	if (!m_bInit)
	{
		if (!Init())
		{
			// error
			printf("Init failed.\n");
			return;
		}
	}

	// Parse address
	m_strFullUrl = addr;
	if (m_strFullUrl.Left(7).Compare(L"http://") == 0)
		m_strFullUrl = m_strFullUrl.Mid(7);
	else
	{
		// TODO 다른 prefix 인지 체크
	}

	int nIndex = m_strFullUrl.Find(L"/");
	if (nIndex > 0)
	{
		m_strHost = m_strFullUrl.Left(nIndex);
		m_strDirectory = m_strFullUrl.Mid(nIndex);
	}
	else
	{
		m_strHost = m_strFullUrl;
		m_strDirectory = L"/";
	}

	nIndex = m_strHost.Find(L":");
	if (nIndex > 0)
	{
		m_strPort = m_strHost.Right(m_strHost.GetLength() - nIndex - 1);
		m_strHost = m_strHost.Left(nIndex);
	}
	else
		m_strPort = L"80";

	// Setup struct
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	addrinfo *result;
	int ret = getaddrinfo(m_strHost.cstr(), m_strPort.cstr(), &hints, &result);
	if (ret != 0)
	{
		// error
		printf("getaddrinfo failed\n");
		return;
	}

	m_ConnectSocket = INVALID_SOCKET;
	char szHostName[NI_MAXHOST];
	if (result)
	{
		ret = getnameinfo(result->ai_addr, sizeof(sockaddr), szHostName, sizeof(szHostName), NULL, 0, 0);
		if (ret != 0)
		{
			// TODO error
			printf("getnameinfo failed\n");
			return;
		}
	}

	// Attempt to connect to an address until one succeeds
	addrinfo *ptr;
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		m_ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (m_ConnectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			Close();
			return;
		}

		// Connect to server.
		ret = connect(m_ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (ret == SOCKET_ERROR)
		{
			closesocket(m_ConnectSocket);
			m_ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (m_ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		Close();
		return;
	}

	m_bConnected = true;
}

void NaHttp::Close()
{
	if (m_bConnected)
	{
		closesocket(m_ConnectSocket);
		m_bConnected = false;
	}

	if (m_bInit)
	{
		WSACleanup();
		m_bInit = false;
	}
}

void NaHttp::SendRequest()
{
	// Send an initial buffer
	NaString strBuf;
	switch (m_enReqMethod)
	{
	case NA_HTTP_GET:
		strBuf.Format(L"GET %s\r\n"
			"User-Agent: NaMacroJS\r\n"
			"Content-Type: application/text\r\n"
			"Accept-Charset: utf-8\r\n"
			, m_strDirectory);
		break;
	case NA_HTTP_POST:
		{
			int nContentLength = 0;
			strBuf.Format(
				L"POST %s HTTP / 1.1\r\n"
				"Host: %s\r\n"
				"User-Agent: NaMacroJS\r\n"
				"Content-Length: %d\r\n"
				"Content-Type: application/text\r\n"
				"Accept-Charset: utf-8\r\n\r\n"
				//"dat=somedata\r\n",
				"",
				m_strDirectory,
				m_strHost,
				nContentLength);
		}
		break;
	}
	
	int ret = send(m_ConnectSocket, strBuf.cstr(), (int)strBuf.GetLength(), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		Close();
		return;
	}

	printf("Bytes Sent: %ld\n", ret);

	// shutdown the connection since no more data will be sent
	ret = shutdown(m_ConnectSocket, SD_SEND);
	if (ret == SOCKET_ERROR) 
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		Close();
		return;
	}

	// Receive until the peer closes the connection
#define DEFAULT_BUFLEN 512
	char *recvbuf = new char[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	m_strResponse = L"";
	do {
		ret = recv(m_ConnectSocket, recvbuf, recvbuflen, 0);
		if (ret > 0)
		{
			printf("Bytes received: %d\n", ret);
			*(((char*)recvbuf) + ret) = 0;

			m_strResponse += NaString(recvbuf);
		}
		else if (ret == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (ret > 0);

	// cleanup
	Close();
}

NaString NaHttp::GetResponse()
{
	return m_strResponse;
}

void NaHttp::AddHeaderParam(const wchar_t * name, const wchar_t * value)
{
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(value);
	/*
	NaString strName(name);
	NaString strValue(value);

	m_mapHeader.insert(
		std::pair<NaString, NaString>(strName, strValue)
	);
	*/
}
