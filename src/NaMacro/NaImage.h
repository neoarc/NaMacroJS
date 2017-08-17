#pragma once

#include "V8Wrap.h"
#include "JsObjectBase.h"

#include <Windows.h>
#include <map>

class NaImage : public JsObjectBase
{
public:
	NaImage();
	virtual ~NaImage();

	// members
	long m_lUID;
	HDC m_hMemoryDC;
	HBITMAP m_hBitmap;
	RECT m_rc;

	POINT FindColor(DWORD dwColor);
	bool Save(const wchar_t *filename);

	// static
	static Local<Object> CreateV8Image(Isolate *isolate, NaImage *pImage = NULL);
	static NaImage* CaptureScreen(int x, int y, int width, int height);
	static NaImage* Load(const wchar_t *filename);
	static int GetEncoderClsid(const wchar_t *format, CLSID *pClsid);
	static LPCOLORREF ImageToBuffer(NaImage *pImage);
	static POINT SearchImageInImage(NaImage *pTarget, NaImage *pSource, int nAccuracyFactor = 0);
	
	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_NaImageTemplate; }
	static Global<ObjectTemplate> s_NaImageTemplate;

	// accessors
	DEFINE_CLASS_ACCESSOR(width);
	DEFINE_CLASS_ACCESSOR(height);

	// methods
	DEFINE_CLASS_METHOD(constructor);
	DEFINE_CLASS_METHOD(getPixel);
	DEFINE_CLASS_METHOD(findImage);
	DEFINE_CLASS_METHOD(reset);
	DEFINE_CLASS_METHOD(save);
};