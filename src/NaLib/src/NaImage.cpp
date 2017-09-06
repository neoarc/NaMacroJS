#include "stdafx.h"
#include "NaImage.h"

#include "NaString.h"
#include "NaDebug.h"
#include "NaDesktop.h"

#pragma warning(push)
	#pragma warning(disable:4458)
	#include <gdiplus.h>
#pragma warning(pop)
#pragma comment(lib, "gdiplus.lib")

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

POINT NaImage::FindColor(DWORD dwColor)
{
	POINT pt;
	pt.x = -1;
	pt.y = -1;

	if (m_hMemoryDC && m_hBitmap)
	{
		HGDIOBJ hOldBitmap = ::SelectObject(m_hMemoryDC, m_hBitmap);
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
					::SelectObject(m_hMemoryDC, hOldBitmap);
					return pt;
				}
			}
		}

		::SelectObject(m_hMemoryDC, hOldBitmap);
	}

	return pt;
}

// description: Save NaImage to file using Gdi+
bool NaImage::Save(const wchar_t *filename)
{
	if (filename)
	{
		// initialize gdi+
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		HPALETTE hPalette = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

		Gdiplus::Bitmap *pBitmap = new Gdiplus::Bitmap((HBITMAP)m_hBitmap, hPalette);

		CLSID clsid;
		NaString strFileName(filename);
		if (strFileName.Right(4).CompareNoCase(L".bmp") == 0)
			GetEncoderClsid(L"image/bmp", &clsid);
		else if (strFileName.Right(4).CompareNoCase(L".jpg") == 0)
			GetEncoderClsid(L"image/jpeg", &clsid);
		else if (strFileName.Right(4).CompareNoCase(L".png") == 0)
			GetEncoderClsid(L"image/png", &clsid);
		else
		{
			// Not supported format
			delete pBitmap;
			return false;
		}

		// TODO check path exists.
		pBitmap->Save(strFileName.wstr(), &clsid, NULL);
		delete pBitmap;
		pBitmap = nullptr;
		
		// TODO validate result

		// shutdown gdi+
		Gdiplus::GdiplusShutdown(gdiplusToken);
		return true;
	}

	return false;
}

COLORREF NaImage::GetPixel(int x, int y)
{
	HGDIOBJ hOldBitmap = ::SelectObject(m_hMemoryDC, m_hBitmap);
	COLORREF color = ::GetPixel(m_hMemoryDC, x, y);
	::SelectObject(m_hMemoryDC, hOldBitmap);

	return color;
}

void NaImage::Reset()
{
	if (m_hMemoryDC)
	{
		::DeleteDC(m_hMemoryDC);
		m_hMemoryDC = nullptr;
	}

	if (m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = nullptr;
	}

	m_rc = { 0, 0, 0, 0 };
}

// description: get clsid for Save
int NaImage::GetEncoderClsid(const wchar_t *format, CLSID *pClsid)
{
	UINT nNumEncoders = 0;
	UINT nSize = 0;
	Gdiplus::ImageCodecInfo *pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&nNumEncoders, &nSize);

	int nRet = -1;
	do
	{
		if (nSize == 0)
			break;

		pImageCodecInfo = (Gdiplus::ImageCodecInfo *)(malloc(nSize));
		if (pImageCodecInfo == nullptr)
			break;

		GetImageEncoders(nNumEncoders, nSize, pImageCodecInfo);
		for (UINT i=0; i < nNumEncoders; ++i)
		{
			if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[i].Clsid;
				free(pImageCodecInfo);
				pImageCodecInfo = nullptr;

				nRet = i;
				break;
			}
		}
	} while (false);
	
	if (pImageCodecInfo != nullptr)
	{
		free(pImageCodecInfo);
		pImageCodecInfo = nullptr;
	}

	return nRet;
}

// description: capture screen & create Image object
NaImage* NaImage::CaptureScreen(int x, int y, int width, int height)
{
	NaImage *pImage = new NaImage();

	HDC hDC = NaDesktop::GetDC();
	pImage->m_hMemoryDC = ::CreateCompatibleDC(hDC);
	pImage->m_hBitmap = ::CreateCompatibleBitmap(hDC, width, height);
	HGDIOBJ hOldBitmap = ::SelectObject(pImage->m_hMemoryDC, pImage->m_hBitmap);
	::BitBlt(pImage->m_hMemoryDC, 0, 0, width, height, hDC, x, y, SRCCOPY);
	::SelectObject(pImage->m_hMemoryDC, hOldBitmap);

	// TODO must delete
	//::DeleteDC(pImage->m_hMemoryDC);

	//::ReleaseDC(NaScreenModule::GetDesktopHWND(), hDC);

	pImage->m_rc.left = x;
	pImage->m_rc.top = y;
	pImage->m_rc.right = x + width;
	pImage->m_rc.bottom = y + height;

	return pImage;
}

// description: load image from file using gdi+
NaImage * NaImage::Load(const wchar_t * filename)
{
	NaImage *pImage = new NaImage;
	HDC hDC = NaDesktop::GetDC();
	pImage->m_hMemoryDC = ::CreateCompatibleDC(hDC);

	// initialize gdi+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// load image
	Gdiplus::Image* pGdiImage = new Gdiplus::Image(filename);

	// converting to bitmap
	Gdiplus::Bitmap* pGdiBitmap = static_cast<Gdiplus::Bitmap*>(pGdiImage);
	pGdiBitmap->GetHBITMAP(Gdiplus::Color(0, 0, 0), &pImage->m_hBitmap);

	pImage->m_rc.left = 0;
	pImage->m_rc.top = 0;
	pImage->m_rc.right = pGdiImage->GetWidth();
	pImage->m_rc.bottom = pGdiImage->GetHeight();

	// shutdown gdi+
	delete pGdiImage;
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return pImage;
}

// description: Make image object to byte array
LPCOLORREF NaImage::ImageToBuffer(NaImage * pImage)
{
	if (pImage == nullptr)
		return nullptr;

	HDC hCompatibleDC = CreateCompatibleDC(pImage->m_hMemoryDC);
	if (!hCompatibleDC)
		return nullptr;

	LPCOLORREF buf = nullptr;
	HGDIOBJ hOldObj = nullptr;
	bool bFail = true;
	do 
	{		
		struct BITMAPINFO3
		{
			BITMAPINFOHEADER bmiHeader;
			RGBQUAD bmiColors[260];
		} bmi;

		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biBitCount = 0;
		if (!GetDIBits(hCompatibleDC, pImage->m_hBitmap, 0, 0, nullptr, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS))
			break;

		int nHeight = bmi.bmiHeader.biHeight;

		buf = new DWORD[bmi.bmiHeader.biSizeImage];

		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biHeight = -bmi.bmiHeader.biHeight;
		
		hOldObj = SelectObject(hCompatibleDC, pImage->m_hBitmap);													  
 		if (!(GetDIBits(hCompatibleDC, pImage->m_hBitmap, 0, nHeight, buf, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS)))
			break;

		bFail = false;
	} while (false);

	if (hOldObj)
		SelectObject(hCompatibleDC, hOldObj);
	DeleteDC(hCompatibleDC);
	
	if (bFail && buf)
	{
		delete[] buf;
		buf = nullptr;
	}

	return buf;
}

// description: Search sourch image from target image
POINT NaImage::SearchImageInImage(NaImage * pTarget, NaImage * pSource, int nAccuracyFactor)
{
	POINT pt = { -1, -1 };
	if (pTarget != nullptr && pSource != nullptr)
	{
		int nTargetWidth = pTarget->m_rc.right - pTarget->m_rc.left;
		int nTargetHeight = pTarget->m_rc.bottom - pTarget->m_rc.top;
		int nSourceWidth = pSource->m_rc.right - pSource->m_rc.left;
		int nSourceHeight = pSource->m_rc.bottom - pSource->m_rc.top;

		if (nTargetWidth >= nSourceWidth && nTargetHeight >= nSourceHeight)
		{
			// FindImage Logic V2: Compare memory
			LPCOLORREF lpTargetBuf = ImageToBuffer(pTarget);
			LPCOLORREF lpSourceBuf = ImageToBuffer(pSource);

			if (lpTargetBuf == nullptr || lpSourceBuf == nullptr)
			{
				if (lpTargetBuf)
					delete[] lpTargetBuf;
				if (lpSourceBuf)
					delete[] lpSourceBuf;
				return pt;
			}

			bool bMismatch;
			for (int y = 0; y < nTargetHeight - nSourceHeight; y++)
			{
				for (int x = 0; x < nTargetWidth - nSourceWidth; x++)
				{
					bMismatch = false;
					for (int y2 = 0; y2 < nSourceHeight; y2++)
					{
						for (int x2 = 0; x2 < nSourceWidth; x2++)
						{
							COLORREF nCol1 = lpTargetBuf[(x + x2) + (y + y2) * (pTarget->m_rc.right - pTarget->m_rc.left)];
							COLORREF nCol2 = lpSourceBuf[(x2) + (y2) * (pSource->m_rc.right - pSource->m_rc.left)];
							if (nAccuracyFactor == 0)
							{
								if (nCol1 != nCol2)
								{
									bMismatch = true;
									break;
								}
							}
							else
							{
								BYTE r1 = GetRValue(nCol1);
								BYTE g1 = GetGValue(nCol1);
								BYTE b1 = GetBValue(nCol1);
								BYTE r2 = GetRValue(nCol2);
								BYTE g2 = GetGValue(nCol2);
								BYTE b2 = GetBValue(nCol2);
								if (abs(r1 - r2) > nAccuracyFactor ||
									abs(g1 - g2) > nAccuracyFactor ||
									abs(b1 - b2) > nAccuracyFactor)
								{
									bMismatch = true;
									break;
								}
							}
						}
						if (bMismatch)
							break;
					}

					if (bMismatch == false)
					{
						pt.x = x;
						pt.y = y;
						if (lpTargetBuf)
							delete[] lpTargetBuf;
						if (lpSourceBuf)
							delete[] lpSourceBuf;

						return pt;
					}
				}
			}

			if (lpTargetBuf)
				delete[] lpTargetBuf;
			if (lpSourceBuf)
				delete[] lpSourceBuf;

			// FindImage Logic V1
			/*
			HGDIOBJ hObjTarget = ::SelectObject(pTarget->m_hMemoryDC, pTarget->m_hBitmap);
			HGDIOBJ hObjSource = ::SelectObject(pSource->m_hMemoryDC, pSource->m_hBitmap);

			bool bMismatch;
			for (int y = 0; y < nTargetHeight - nSourceHeight; y++)
			{
				for (int x = 0; x < nTargetWidth - nSourceWidth; x++)
				{
					bMismatch = false;
					for (int y2 = 0; y2 < nSourceHeight; y2++)
					{
						for (int x2 = 0; x2 < nSourceWidth; x2++)
						{
							COLORREF nCol1 = ::GetPixel(pTarget->m_hMemoryDC, x + x2, y + y2);
							COLORREF nCol2 = ::GetPixel(pSource->m_hMemoryDC, x2, y2);
							if (nCol1 != nCol2)
							{
								bMismatch = true;
								break;
							}
						}
						if (bMismatch)
							break;
					}

					if (bMismatch == false)
					{
						pt.x = x;
						pt.y = y;
						::SelectObject(pTarget->m_hMemoryDC, hObjTarget);
						::SelectObject(pSource->m_hMemoryDC, hObjSource);

						return pt;
					}
				}
			}

			::SelectObject(pTarget->m_hMemoryDC, hObjTarget);
			::SelectObject(pSource->m_hMemoryDC, hObjSource);
			*/
		}
	}

	return pt;
}
