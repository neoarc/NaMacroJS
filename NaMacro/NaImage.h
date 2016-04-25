#pragma once

#include "Common.h"

#include <Windows.h>
#include <map>

class NaImage 
{
public:
	NaImage(long lUID = -1);
	~NaImage();

	// members
	long m_lUID;
	HDC m_hMemoryDC;
	HBITMAP m_hBitmap;
	RECT m_rc;

	// management image list
	static long s_lUniqueID;
	static long CreateUniqueID();
	static std::map<long, NaImage*> s_mapImage;
	static void DestroyMap();

	// static
	static v8::Local<v8::Object> CreateV8Image(v8::Isolate *isolate, NaImage *pImage = NULL);

	static NaImage* CaptureScreen(int x, int y, int width, int height);

	// static - Convert between NaImage/V8
	// NaWindow -> V8Window
	static v8::Local<v8::Object> GetV8Image(v8::Isolate *isolate, NaImage *pImage);

	static NaImage* GetNaImage(v8::Isolate *isolate, v8::Local<v8::Object> obj);

	// methods
	DEFINE_CLASS_METHOD(GetPixel);
};