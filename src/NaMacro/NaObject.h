#pragma once

#include "V8Wrap.h"

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

	// #FIXME: open this after complete v8 on nuget
//	virtual void MakeWeak();
// 	static void WeakCallback(
// 		const v8::WeakCallbackData<v8::Object, NaObject>& data
// 		//Persistent<Value> obj, void *pData
// 		);
};

