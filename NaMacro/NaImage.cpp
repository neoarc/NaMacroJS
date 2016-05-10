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

	if (m_hMemoryDC)
	{
		::DeleteDC(m_hMemoryDC);
	}

	if (m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
	}
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
	ADD_IMAGE_ACCESSOR(width, GetWidth, SetWidth);
	ADD_IMAGE_ACCESSOR(height, GetHeight, SetHeight);

	// methods
	ADD_IMAGE_METHOD(getPixel, GetPixel);

	return handle_scope.Escape(templ);
}

POINT NaImage::FindColor(DWORD dwColor)
{
	POINT pt;
	pt.x = -1;
	pt.y = -1;

	if (m_hMemoryDC && m_hBitmap)
	{
		COLORREF color;
		int nWidth = m_rc.right - m_rc.left;
		int nHeight = m_rc.bottom - m_rc.top;
		for (int x = 0; x < nWidth; x++)
		{
			for (int y = 0; y < nHeight; y++)
			{
				color = ::GetPixel(m_hMemoryDC, x, y);
				if (color == dwColor)
				{
					pt.x = x;
					pt.y = y;
					return pt;
				}
			}
		}
	}

	return pt;
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
	//::DeleteDC(pImage->m_hMemoryDC);

	//::ReleaseDC(NaScreenModule::GetDesktopHWND(), hDC);

	pImage->m_rc.left = x;
	pImage->m_rc.top = y;
	pImage->m_rc.right = x + width;
	pImage->m_rc.bottom = y + height;

	return pImage;
}

// description: width property getter/setter
void NaImage::GetWidth(V8_GETTER_ARGS)
{
	NaImage *pImage = reinterpret_cast<NaImage*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	int nWidth = 0;
	if (pImage)
	{
		nWidth = pImage->m_rc.right - pImage->m_rc.left;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, nWidth)
		);
}

void NaImage::SetWidth(V8_SETTER_ARGS)
{
	NaImage *pImage = reinterpret_cast<NaImage*>(UnwrapObject(info.This()));
	if (pImage)
	{
		// Not Impl
	}
}

// description: height property getter/setter
void NaImage::GetHeight(V8_GETTER_ARGS)
{
	NaImage *pImage = reinterpret_cast<NaImage*>(UnwrapObject(info.This()));
	Isolate *isolate = info.GetIsolate();
	int nHeight = 0;
	if (pImage)
	{
		nHeight = pImage->m_rc.bottom - pImage->m_rc.top;
	}

	info.GetReturnValue().Set(
		Integer::New(isolate, nHeight)
		);
}

void NaImage::SetHeight(V8_SETTER_ARGS)
{
	NaImage *pImage = reinterpret_cast<NaImage*>(UnwrapObject(info.This()));
	if (pImage)
	{
		// Not Impl
	}
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
