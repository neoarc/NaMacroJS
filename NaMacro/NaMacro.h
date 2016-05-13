#pragma once

#include "vector"
#include "ModuleBase.h"

using namespace std;

extern bool g_bReportExceptions;
extern bool g_bExit;

// prototypes
void CreateDefaultModules(Isolate *isolate, Local<ObjectTemplate> &global_template);
void InitModules(Isolate *isolate, Local<ObjectTemplate> &global_template);
void ReleaseModules();

vector<ModuleBase*> g_ModuleList;

// Buffer Allocator
class ArrayBufferAllocator : public ArrayBuffer::Allocator {
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