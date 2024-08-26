#pragma once

class NaImage;
class NaScreen
{
public:
	static int GetWidth();
	static int GetHeight();

	static int GetPixel(int x, int y);
	static int SetAero(bool bEnable);
};

