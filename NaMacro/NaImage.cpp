#include "NaImage.h"

#include "ScreenModule.h"

Global<ObjectTemplate> NaImage::s_NaImageTemplate;

NaImage::NaImage()
{
	m_hMemoryDC = NULL;
	NaDebugOut(L"NaImage(): 0x%08x\n", this);
}

NaImage::~NaImage()
{
	NaDebugOut(L"~NaImage(): 0x%08x\n", this);
}

Local<ObjectTemplate> NaImage::MakeObjectTemplate(Isolate * isolate)
{
	EscapableHandleScope handle_scope(isolate);

	Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
	templ->SetInternalFieldCount(1);

	// bind image methods
#define ADD_IMAGE_ACCESSOR(_prop, _getter, _setter)	ADD_OBJ_ACCESSOR(templ, _prop, _getter, _setter);
#define ADD_IMAGE_METHOD(_js_func, _c_func)			ADD_TEMPLATE_METHOD(templ, _js_func, _c_func);

	// accessor
	//ADD_IMAGE_ACCESSOR(x, GetX, SetX);

	// methods
	ADD_IMAGE_METHOD(getPixel, GetPixel);

	return handle_scope.Escape(templ);
}

// description: capture screen & create Image object
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

// description: get pixel from image buffer
// syntax:		imageObj.getPixel(x, y);
void NaImage::GetPixel(V8_FUNCTION_ARGS)
{
	Isolate *isolate = args.GetIsolate();
	Local<Object> obj = args.This();
	NaImage *pImage = reinterpret_cast<NaImage*>(UnwrapObject(obj));
	if (pImage == nullptr)
	{
		// error
		args.GetReturnValue().Set(Integer::New(isolate, -1));
		return;
	}

	if (args.Length() < 2)
	{
		// error
		args.GetReturnValue().Set(Integer::New(isolate, -1));
		return;
	}

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();

	COLORREF color = ::GetPixel(pImage->m_hMemoryDC, x, y);

	// return
	args.GetReturnValue().Set(Integer::New(isolate, color));
}
