#pragma once

#include "Common.h"
#include "NaObject.h"

#include <Windows.h>
#include <map>

class NaImage : public NaObject
{
public:
	NaImage();
	virtual ~NaImage();

	// members
	long m_lUID;
	HDC m_hMemoryDC;
	HBITMAP m_hBitmap;
	RECT m_rc;

	// static
	static Local<Object> CreateV8Image(Isolate *isolate, NaImage *pImage = NULL);
	static NaImage* CaptureScreen(int x, int y, int width, int height);
	
	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_NaImageTemplate; }
	static Global<ObjectTemplate> s_NaImageTemplate;

	// methods
	DEFINE_CLASS_METHOD(GetPixel);
};