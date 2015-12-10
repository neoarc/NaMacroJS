#pragma once

#include "vector"
#include "NaModuleBase.h"

using namespace std;

void ReportException(v8::Isolate *isolate, v8::TryCatch* handler);
bool report_exceptions = true;

extern bool g_bExit;

// Modulize
void InitModules(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template, int nPhase);
void ReleaseModules();
vector<NaModuleBase*> g_ModuleList;

// Buffer Allocator
class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
public:
	virtual void* Allocate(size_t length) {
		void* data = AllocateUninitialized(length);
		return data == NULL ? data : memset(data, 0, length);
	}
	virtual void* AllocateUninitialized(size_t length) {
		return malloc(length);
	}
	virtual void Free(void* data, size_t) {
		free(data);
	}
};