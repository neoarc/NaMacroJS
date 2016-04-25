#include "NaImage.h"

#include "ScreenModule.h"

long NaImage::s_lUniqueID = 0;
std::map<long, NaImage*> NaImage::s_mapImage;

NaImage::NaImage(long lUID)
{
	m_hMemoryDC = NULL;
	if (lUID == -1)
	{
		lUID = NaImage::CreateUniqueID();
	}

	m_lUID = lUID;

	s_mapImage.insert(std::pair<long, NaImage*>(lUID, this));
	NaDebugOut(L"NaImage(): 0x%08x\n", this);
}

NaImage::~NaImage()
{
	NaDebugOut(L"~NaImage(): 0x%08x\n", this);
}

long NaImage::CreateUniqueID()
{
	return s_lUniqueID++;
}

void NaImage::DestroyMap()
{
	int nSize = NaImage::s_mapImage.size();
	std::map<long, NaImage*>::iterator it = NaImage::s_mapImage.begin();
	for (; it != NaImage::s_mapImage.end(); it++)
	{
		NaImage *pWindow = it->second;
		delete pWindow;
	}

	NaImage::s_mapImage.clear();
}

v8::Local<v8::Object> NaImage::CreateV8Image(v8::Isolate * isolate, NaImage *pImage)
{
	v8::Local<v8::Object> obj = v8::Object::New(isolate);

	// TODO bind api to prototype (not object)

	// bind image methods
#define ADD_IMAGE_ACCESSOR(_prop, _getter, _setter)	ADD_OBJ_ACCESSOR(obj, _prop, _getter, _setter);
#define ADD_IMAGE_METHOD(_js_func, _c_func)			ADD_OBJ_METHOD(obj, _js_func, _c_func);

	// accessor
	//ADD_IMAGE_ACCESSOR(x, GetX, SetX);

	// methods
	ADD_IMAGE_METHOD(getPixel, GetPixel);

	long lUID = -1;
	if (pImage == NULL)
	{
		lUID = NaImage::CreateUniqueID();
		pImage = new NaImage(lUID);
	}
	else
	{
		lUID = pImage->m_lUID;
	}

	obj->Set(
		v8::String::NewFromUtf8(isolate, "_unique_id", v8::NewStringType::kNormal).ToLocalChecked(),
		v8::Number::New(isolate, lUID)
		);

	return obj;
}

NaImage* NaImage::CaptureScreen(int x, int y, int width, int height)
{
	NaImage *pImage = new NaImage();

	HDC hDC = NaScreenModule::GetDesktopDC();
	pImage->m_hMemoryDC = ::CreateCompatibleDC(hDC);
	pImage->m_hBitmap = ::CreateCompatibleBitmap(hDC, width, height);
	::SelectObject(pImage->m_hMemoryDC, pImage->m_hBitmap);
	::BitBlt(pImage->m_hMemoryDC, 0, 0, width, height, hDC, x, y, SRCCOPY);

	// TODO must delete
	//::DeleteDC(m_hMemoryDC);

	//::ReleaseDC(NaScreenModule::GetDesktopHWND(), hDC);

	pImage->m_rc.left = x;
	pImage->m_rc.top = y;
	pImage->m_rc.right = x + width;
	pImage->m_rc.bottom = y + height;

	return pImage;
}

// description: wrap NaImage to V8 Object
v8::Local<v8::Object> NaImage::GetV8Image(v8::Isolate * isolate, NaImage * pImage)
{
	auto obj = CreateV8Image(isolate, pImage);
	return obj;
}

NaImage * NaImage::GetNaImage(v8::Isolate * isolate, v8::Local<v8::Object> obj)
{
	NaImage *pImage = NULL;

	v8::Local<v8::Value> value = obj->Get(v8::String::NewFromUtf8(isolate, "_unique_id", v8::NewStringType::kNormal).ToLocalChecked());
	long lUID = value->Int32Value();
	std::map<long, NaImage*>::iterator it = s_mapImage.find(lUID);
	if (it != s_mapImage.end())
	{
		pImage = it->second;
	}

	return pImage;
}

// description: get pixel from image buffer
// syntax:		imageObj.getPixel(x, y);
void NaImage::GetPixel(V8_FUNCTION_ARGS)
{
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Object> obj = args.This();
	NaImage *pImage = GetNaImage(isolate, obj);
	if (pImage == nullptr)
	{
		// error
		args.GetReturnValue().Set(v8::Integer::New(isolate, -1));
		return;
	}

	if (args.Length() < 2)
	{
		// error
		args.GetReturnValue().Set(v8::Integer::New(isolate, -1));
		return;
	}

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	COLORREF color = ::GetPixel(pImage->m_hMemoryDC, x, y);

	// return
	args.GetReturnValue().Set(v8::Integer::New(isolate, color));
}
