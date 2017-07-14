#include "stdafx.h"

#include "NaCurl.h"

#include "NaDebug.h"

/*
#pragma comment(lib, "./CurlLib/lib/libcurl.lib")
#pragma comment(lib, "./CurlLib/lib/libeay32.lib")
#pragma comment(lib, "./CurlLib/lib/ssleay32.lib")
*/

NaCurl::NaCurl()
{
	curl_ios<ostringstream> writer(m_ostrOutput);
	writer.set_callback(NaCurl::write_callback);

	// Pass the writer to the easy constructor and watch the content returned in that variable!
	m_pCurlEasy = new curl_easy(writer);
	m_lLastError = 0;
	m_strLastError = L"";

	NaDebug::Out(L"this: 0x%08x\n", this);
}

NaCurl::~NaCurl()
{
	if (m_pCurlEasy)
	{
		delete m_pCurlEasy;
		m_pCurlEasy = nullptr;
	}
}

NaString NaCurl::Post(NaString strUrl, NaString strBody)
{
	NaDebug::Out(L"URL: %ls\n", strUrl.wstr());

	// Add some option to the easy handle
	m_pCurlEasy->add<CURLOPT_URL>(strUrl.cstr());
	m_pCurlEasy->add<CURLOPT_FOLLOWLOCATION>(1L);
	m_pCurlEasy->add<CURLOPT_SSL_VERIFYPEER>(0);

	if (strBody.GetLength() > 0)
	{
		m_pCurlEasy->add<CURLOPT_POSTFIELDS>(strBody.cstr());
	}

	try
	{
		// Execute the request.
		m_ostrOutput.str("");
		m_ostrOutput.clear();
		m_pCurlEasy->perform();
	}
	catch (curl_easy_exception error)
	{
		// If you want to get the entire error stack we can do:
		curlcpp_traceback errors = error.get_traceback();
		
		// Otherwise we could print the stack like this:
		error.print_traceback();

		// #TODO Exception handling

		/*
		std::for_each(curl_exception::traceback.begin(), curl_exception::traceback.end(), [](const curlcpp_traceback_object &value) {
			std::cout << "ERROR: " << value.first << " ::::: FUNCTION: " << value.second << std::endl;
		});
		*/

		//throw NaException(__FUNCTIONW__, __LINE__, L"NaCurl internal exception.");

		return L"";
	}

	// Let's print the stream content.
	const std::string str = m_ostrOutput.str();
	const char* cstr = str.c_str();

	NaString strRet = cstr;
	if (strRet.GetLength() > 80)
		NaDebug::Out(L"RET: %ls ...(skip)\n", strRet.Left(80).wstr());
	else
		NaDebug::Out(L"RET: %ls\n", strRet.wstr());

	return strRet;
}

NaString NaCurl::Put(NaString strUrl, NaString strBody)
{
	if (strBody.GetLength() == 0)
		return L"";

	NaDebug::Out(L"URL: %ls\n", strUrl.wstr());

	// Add some option to the easy handle
	m_pCurlEasy->add<CURLOPT_URL>(strUrl.cstr());
	m_pCurlEasy->add<CURLOPT_FOLLOWLOCATION>(1L);
	m_pCurlEasy->add<CURLOPT_SSL_VERIFYPEER>(0);
	m_pCurlEasy->add<CURLOPT_CUSTOMREQUEST>("PUT");

	m_pCurlEasy->add<CURLOPT_POSTFIELDS>(strBody.cstr());

	try
	{
		// Execute the request.
		m_ostrOutput.str("");
		m_ostrOutput.clear();
		m_pCurlEasy->perform();
	}
	catch (curl_easy_exception error)
	{
		// If you want to get the entire error stack we can do:
		curlcpp_traceback errors = error.get_traceback();
		// Otherwise we could print the stack like this:
		error.print_traceback();
	}

	// Let's print the stream content.
	const std::string str = m_ostrOutput.str();
	const char* cstr = str.c_str();

	NaString strRet = cstr;
	NaDebug::Out(L"RET: %ls\n", strRet.wstr());

	return strRet;
}

bool NaCurl::Get(NaString strUrl, char **outBuf, long &lSize)
{
	NaDebug::Out(L"URL: %ls\n", strUrl.wstr());

	// Add some option to the easy handle
	m_pCurlEasy->add<CURLOPT_URL>(strUrl.cstr());
	m_pCurlEasy->add<CURLOPT_FOLLOWLOCATION>(1L);
	m_pCurlEasy->add<CURLOPT_NOSIGNAL>(1L);
	m_pCurlEasy->add<CURLOPT_ACCEPT_ENCODING>("deflate");
	
	try
	{
		// Execute the request.
		m_ostrOutput.str("");
		m_ostrOutput.clear();
		m_pCurlEasy->perform();
	}
	catch (curl_easy_exception error)
	{
		// If you want to get the entire error stack we can do:
		curlcpp_traceback errors = error.get_traceback();
		// Otherwise we could print the stack like this:
		error.print_traceback();

		return false;
	}

	// Let's print the stream content.
	const std::string str = m_ostrOutput.str();
	const char* cstr = str.c_str();

	if ((size_t)lSize != -1 && str.size() != (size_t)lSize)
	{
		m_strLastError.Format(L"Size mismatch: %d, %d", str.size(), lSize);
		return false;
	}
	else
	{
		lSize = str.size();
		if (*outBuf == nullptr)
			*outBuf = new char[lSize];
		memcpy(*outBuf, cstr, lSize);
	}

	return true;
}

unsigned long NaCurl::GetLastError()
{
	return m_lLastError;
}

NaString NaCurl::GetLastErrorMessage()
{
	return m_strLastError;
}

size_t NaCurl::write_data(void *ptr, size_t size, size_t nmemb, void *stream) 
{
	std::string data((const char*)ptr, (size_t)size * nmemb);
	*((std::stringstream*)stream) << data << endl;

	return size * nmemb;
}

size_t NaCurl::write_callback(void * contents, size_t size, size_t nmemb, void * userp)
{
	// Temporary code: Working in progress
	static long lTotal = 0;
	lTotal += (size * nmemb);
	NaDebug::Out(L"callback: %d (%ld)\n", size * nmemb, lTotal);

	NaCurl* pCurl = reinterpret_cast<NaCurl*>(userp);
	std::string data((const char*)contents, (size_t)size * nmemb);
	pCurl->m_ostrOutput << data << endl;

	return size * nmemb;
}

void NaCurl::ClearLastError()
{
	m_lLastError = 0;
	m_strLastError = L"";
}
