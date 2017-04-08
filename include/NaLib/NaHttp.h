//
// Http protocol transfer 
// 2016.06.24 neoarc
//
#pragma once

#include <WinSock2.h>
#include "NaString.h"

#include <map>

enum NaHttpReqTypes
{
	NA_HTTP_GET,
	NA_HTTP_POST,
};

class NaHttp
{
public:
	NaHttp();
	~NaHttp();

	bool Init();
	void Open(const wchar_t* addr);
	void Close();

	void SendRequest();
	NaString GetResponse();

	void AddHeaderParam(const wchar_t* name, const wchar_t* value);
	void AddContentParam(const wchar_t* name, const wchar_t* value);

protected:
	// Status
	bool m_initialized;
	bool m_connected;

	// Connection info
	NaString m_fullUrl;
	NaString m_host;
	NaString m_directory;
	NaString m_port;
	SOCKET m_socket;
	
	// Request
	NaHttpReqTypes m_enReqMethod;
	std::map<NaString, NaString>m_mapHeader;

	// Response
	NaString m_response;
};

