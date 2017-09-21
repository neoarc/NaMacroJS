#pragma once

#include "V8Wrap.h"

class JsObjectBase
{
public:
	JsObjectBase();
	virtual ~JsObjectBase();

	virtual int AddRef();
	virtual int Release();
	int m_nRef;
	Persistent<Object> m_Persistent;

	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_JsObjectTemplate; }
	static Global<ObjectTemplate> s_JsObjectTemplate;

	static Local<Object> WrapObject(Isolate *isolate, JsObjectBase *pObject);
	static JsObjectBase* UnwrapObject(Local<Object> obj);

	// #FIXME: open this after complete v8 on nuget
	virtual void MakeWeak();
	static void WeakCallback(
		const v8::WeakCallbackInfo<JsObjectBase>& data
		//Persistent<Value> obj, void *pData
		);
};

