// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#ifndef UNUSED
#	define UNUSED(a)	(a)
#endif

#pragma warning(push)
#	pragma warning(disable:4100)
#	include <v8.h>
#	include <libplatform.h>
#pragma warning(pop)

using namespace v8;