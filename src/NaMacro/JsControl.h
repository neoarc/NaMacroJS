#pragma once

#include "JsObjectBase.h"
#include "V8Wrap.h"

#include <map>

class NaWindow;
class NaControl;
class JsControl : public JsObjectBase
{
public:
	JsControl();
	~JsControl();

	void Create(V8_METHOD_ARGS, NaWindow *pParent);
	void Destroy();

	// member
	NaControl* m_pNativeControl;

	// static
	static NaControl* UnwrapNativeControl(Local<Object> obj);

	// statics
	static std::map<HWND, Persistent<Function, CopyablePersistentTraits<Function>>> s_mapControlCallback;
	static void OnCommand(Isolate *isolate, HWND hWnd, int nCode);

	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_JsControlTemplate; };
	static Global<ObjectTemplate> s_JsControlTemplate;

	// accessors
	DEFINE_CLASS_PROPERTY(x);
	DEFINE_CLASS_PROPERTY(y);
	DEFINE_CLASS_PROPERTY(width);
	DEFINE_CLASS_PROPERTY(height);
	DEFINE_CLASS_PROPERTY(text);
	DEFINE_CLASS_PROPERTY(visible);
	DEFINE_CLASS_PROPERTY_RO(parent);
	DEFINE_CLASS_PROPERTY(image);
	DEFINE_CLASS_PROPERTY(callback);

	// methods
	DEFINE_CLASS_METHOD(focus);
};

