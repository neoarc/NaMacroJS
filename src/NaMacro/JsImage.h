#pragma once

#include "V8Wrap.h"
#include "JsObjectBase.h"

#include <Windows.h>
#include <map>

class NaImage;
class JsImage : public JsObjectBase
{
public:
	JsImage();
	virtual ~JsImage();

	// members
	NaImage *m_pNativeImage;

	// static
	static NaImage* UnwrapNativeImage(Local<Object> obj);

	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_JsImageTemplate; }
	static Global<ObjectTemplate> s_JsImageTemplate;

	// accessors
	DEFINE_CLASS_PROPERTY(width);
	DEFINE_CLASS_PROPERTY(height);

	// methods
	DEFINE_CLASS_METHOD(constructor);
	DEFINE_CLASS_METHOD(getPixel);
	DEFINE_CLASS_METHOD(findImage);
	DEFINE_CLASS_METHOD(reset);
	DEFINE_CLASS_METHOD(save);
};