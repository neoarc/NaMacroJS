#pragma once

#if defined(WIN32)
	#include <Windows.h>
#else
	#define CP_ACP                    0
	#define CP_UTF8                   65001       // UTF-8 translation
#endif

#define NASTRING_FORMAT_BUFFER_SIZE	65535