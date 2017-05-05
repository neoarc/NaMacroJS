#include "stdafx.h"
#include "NaHttp.h"

#include <Ws2tcpip.h>

NaHttp::NaHttp()
{
	m_initialized = false;
	m_connected = false;
	m_socket = INVALID_SOCKET;
	m_enReqMethod = NA_HTTP_GET;
}

NaHttp::~NaHttp()
{
	WSACleanup();
}

bool NaHttp::Init()
{
	if (m_initialized)
		return true;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		// TODO error
		printf("WSAStartup failed\n");
		return false;
	}

	return m_initialized = true;
}

void NaHttp::Open(const wchar_t* addr)
{
	if (!m_initialized)
	{
		if (!Init())
		{
			// error
			printf("Init failed.\n");
			return;
		}
	}

	// Parse address
	m_fullUrl = addr;
	if (m_fullUrl.Left(7).Compare(L"http://") == 0)
		m_fullUrl = m_fullUrl.Mid(7);
	else
	{
		// TODO 다른 prefix 인지 체크
	}

	int nIndex = m_fullUrl.Find(L"/");
	if (nIndex > 0)
	{
		m_host = m_fullUrl.Left(nIndex);
		m_directory = m_fullUrl.Mid(nIndex);
	}
	else
	{
		m_host = m_fullUrl;
		m_directory = L"/";
	}

	nIndex = m_host.Find(L":");
	if (nIndex > 0)
	{
		m_port = m_host.Right(m_host.GetLength() - nIndex - 1);
		m_host = m_host.Left(nIndex);
	}
	else
		m_port = L"80";

	// Setup struct
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	addrinfo *result;
	int ret = getaddrinfo(m_host.cstr(), m_port.cstr(), &hints, &result);
	if (ret != 0)
	{
		// error
		printf("getaddrinfo failed\n");
		return;
	}

	m_socket = INVALID_SOCKET;
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
		m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (m_socket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			Close();
			return;
		}

		// Connect to server.
		ret = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (ret == SOCKET_ERROR)
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (m_socket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		Close();
		return;
	}

	m_connected = true;
}

void NaHttp::Close()
{
	if (m_connected)
	{
		closesocket(m_socket);
		m_connected = false;
	}

	if (m_initialized)
	{
		WSACleanup();
		m_initialized = false;
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
			, m_directory);
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
				m_directory,
				m_host,
				nContentLength);
		}
		break;
	}
	
	int ret = send(m_socket, strBuf.cstr(), (int)strBuf.GetLength(), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		Close();
		return;
	}

	printf("Bytes Sent: %ld\n", ret);

	// shutdown the connection since no more data will be sent
	ret = shutdown(m_socket, SD_SEND);
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

	m_response = L"";
	do {
		ret = recv(m_socket, recvbuf, recvbuflen, 0);
		if (ret > 0)
		{
			printf("Bytes received: %d\n", ret);
			*(((char*)recvbuf) + ret) = 0;

			m_response += NaString(recvbuf);
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
	return m_response;
}

void NaHttp::AddHeaderParam(const wchar_t* name, const wchar_t* value)
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
