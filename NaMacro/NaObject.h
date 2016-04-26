#pragma once

#include "Common.h"

#include "v8.h"

#define Global	Global

class NaObject
{
public:
	NaObject();
	virtual ~NaObject();

	virtual int AddRef();
	virtual int Release();
	int m_nRef;
	Persistent<Object> m_Persistent;

	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_NaObjectTemplate; }
	static Global<ObjectTemplate> s_NaObjectTemplate;

	static Local<Object> WrapObject(Isolate *isolate, NaObject *pObject);
	static NaObject* UnwrapObject(Local<Object> obj);

	virtual void MakeWeak();
	static void WeakCallback(
		const v8::WeakCallbackData<v8::Object, NaObject>& data
		//Persistent<Value> obj, void *pData
		);
};

