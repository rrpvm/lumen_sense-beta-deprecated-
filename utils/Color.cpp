#include "Color.h"
DWORD Color::convert(Color color)
{
	DWORD _color;
	int a = color.a;
	int r = color.r;
	int g = color.g;
	int b = color.b;
	a = ((a & 0xff) << 24);
	r = ((r & 0xff) << 16);
	g = ((g & 0xff) << 8);
	b = (b & 0xff);
	_color = DWORD(a | r | g | b);
	return _color;
}

DWORD Color::convert()
{
	DWORD _color;
	int a = this->a;
	int r = this->r;
	int g = this->g;
	int b = this->b;
	a = ((a & 0xff) << 24);
	r = ((r & 0xff) << 16);
	g = ((g & 0xff) << 8);
	b = (b & 0xff);
	_color = DWORD(a | r | g | b);
	return _color;
}
