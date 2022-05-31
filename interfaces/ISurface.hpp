#pragma once

#include "IAppSystem.hpp"
#include "../utils/Color.h"
#include "../utils/Vector.h"
#include <locale>
#include <string>
#include <vector>

namespace vgui
{
	typedef unsigned long HFont;
	typedef unsigned int VPANEL;
};

enum FontFeature
{
	FONT_FEATURE_ANTIALIASED_FONTS = 1,
	FONT_FEATURE_DROPSHADOW_FONTS = 2,
	FONT_FEATURE_OUTLINE_FONTS = 6,
};

enum FontDrawType
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

enum FontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

struct IntRect
{
	int x0;
	int y0;
	int x1;
	int y1;
};

struct vertex_t
{
	vertex_t() { }
	vertex_t(const vec2_t& pos, const vec2_t& coord = vec2_t(0, 0)) {
		position = pos;
		m_TexCoord = coord;
	}
	void Init(const vec2_t& pos, const vec2_t& coord = vec2_t(0, 0)) {
		position = pos;
		m_TexCoord = coord;
	}

	vec2_t	position;
	vec2_t	m_TexCoord;
};

//-----------------------------------------------------------------------------
// Purpose: Wraps contextless windows system functions
//-----------------------------------------------------------------------------
class ISurface : public IAppSystem
{
public:
	virtual void          RunFrame() = 0;
	virtual vgui::VPANEL  GetEmbeddedPanel() = 0;
	virtual void          SetEmbeddedPanel(vgui::VPANEL pPanel) = 0;
	virtual void          PushMakeCurrent(vgui::VPANEL panel, bool useInsets) = 0;
	virtual void          PopMakeCurrent(vgui::VPANEL panel) = 0;
	virtual void          DrawSetColor(int r, int g, int b, int a) = 0;
	virtual void          DrawSetColor(Color col) = 0;
	virtual void          DrawFilledRect(int x0, int y0, int x1, int y1) = 0;
	virtual void          DrawFilledRectArray(IntRect* pRects, int numRects) = 0;
	virtual void          DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;
	virtual void          DrawLine(int x0, int y0, int x1, int y1) = 0;
	virtual void          DrawPolyLine(int* px, int* py, int numPoints) = 0;
	virtual void          DrawSetApparentDepth(float f) = 0;
	virtual void          DrawClearApparentDepth(void) = 0;
	virtual void          DrawSetTextFont(vgui::HFont font) = 0;
	virtual void          DrawSetTextColor(int r, int g, int b, int a) = 0;
	virtual void          DrawSetTextColor(Color col) = 0;
	virtual void          DrawSetTextPos(int x, int y) = 0;
	virtual void          DrawGetTextPos(int& x, int& y) = 0;
	virtual void          DrawPrintText(const wchar_t* text, int textLen, FontDrawType drawType = FontDrawType::FONT_DRAW_DEFAULT) = 0;
	virtual void          DrawUnicodeChar(wchar_t wch, FontDrawType drawType = FontDrawType::FONT_DRAW_DEFAULT) = 0;
	virtual void          DrawFlushText() = 0;
	virtual void* CreateHTMLWindow(void* events, vgui::VPANEL context) = 0;
	virtual void          PaintHTMLWindow(void* htmlwin) = 0;
	virtual void          DeleteHTMLWindow(void* htmlwin) = 0;
	virtual int           DrawGetTextureId(char const* filename) = 0;
	virtual bool          DrawGetTextureFile(int id, char* filename, int maxlen) = 0;
	virtual void          DrawSetTextureFile(int id, const char* filename, int hardwareFilter, bool forceReload) = 0;
	virtual void          DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall) = 0;
	virtual void          DrawSetTexture(int id) = 0;
	virtual void          DeleteTextureByID(int id) = 0;
	virtual void          DrawGetTextureSize(int id, int& wide, int& tall) = 0;
	virtual void          DrawTexturedRect(int x0, int y0, int x1, int y1) = 0;
	virtual bool          IsTextureIDValid(int id) = 0;
	virtual int           CreateNewTextureID(bool procedural = false) = 0;
	virtual void          GetScreenSize(int& wide, int& tall) = 0;
	virtual void          SetAsTopMost(vgui::VPANEL panel, bool state) = 0;
	virtual void          BringToFront(vgui::VPANEL panel) = 0;
	virtual void          SetForegroundWindow(vgui::VPANEL panel) = 0;
	virtual void          SetPanelVisible(vgui::VPANEL panel, bool state) = 0;
	virtual void          SetMinimized(vgui::VPANEL panel, bool state) = 0;
	virtual bool          IsMinimized(vgui::VPANEL panel) = 0;
	virtual void          FlashWindow(vgui::VPANEL panel, bool state) = 0;
	virtual void          SetTitle(vgui::VPANEL panel, const wchar_t* title) = 0;
	virtual void          SetAsToolBar(vgui::VPANEL panel, bool state) = 0;
	virtual void          CreatePopup(vgui::VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true, bool kbInput = true) = 0;
	virtual void          SwapBuffers(vgui::VPANEL panel) = 0;
	virtual void          Invalidate(vgui::VPANEL panel) = 0;
	virtual void          SetCursor(unsigned long cursor) = 0;
	virtual bool          IsCursorVisible() = 0;
	virtual void          ApplyChanges() = 0;
	virtual bool          IsWithin(int x, int y) = 0;
	virtual bool          HasFocus() = 0;
	virtual bool          SupportsFeature(int /*SurfaceFeature_t*/ feature) = 0;
	virtual void          RestrictPaintToSinglePanel(vgui::VPANEL panel, bool bForceAllowNonModalSurface = false) = 0;
	virtual void          SetModalPanel(vgui::VPANEL) = 0;
	virtual vgui::VPANEL  GetModalPanel() = 0;
	virtual void          UnlockCursor() = 0;
	virtual void          LockCursor() = 0;
	virtual void          SetTranslateExtendedKeys(bool state) = 0;
	virtual vgui::VPANEL  GetTopmostPopup() = 0;
	virtual void          SetTopLevelFocus(vgui::VPANEL panel) = 0;
	virtual vgui::HFont   CreateFont_() = 0;
	virtual bool          SetFontGlyphSet(vgui::HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0) = 0;
	virtual bool          AddCustomFontFile(const char* fontFileName) = 0;
	virtual int           GetFontTall(vgui::HFont font) = 0;
	virtual int           GetFontAscent(vgui::HFont font, wchar_t wch) = 0;
	virtual bool          IsFontAdditive(vgui::HFont font) = 0;
	virtual void          GetCharABCwide(vgui::HFont font, int ch, int& a, int& b, int& c) = 0;
	virtual int           GetCharacterWidth(vgui::HFont font, int ch) = 0;
	virtual void          GetTextSize(vgui::HFont font, const wchar_t* text, int& wide, int& tall) = 0;
	virtual vgui::VPANEL  GetNotifyPanel() = 0;
	virtual void          SetNotifyIcon(vgui::VPANEL context, unsigned long icon, vgui::VPANEL panelToReceiveMessages, const char* text) = 0;
	virtual void          PlaySound_(const char* fileName) = 0;
	virtual int           GetPopupCount() = 0;
	virtual vgui::VPANEL  GetPopup(int index) = 0;
	virtual bool          ShouldPaintChildPanel(vgui::VPANEL childPanel) = 0;
	virtual bool          RecreateContext(vgui::VPANEL panel) = 0;
	virtual void          AddPanel(vgui::VPANEL panel) = 0;
	virtual void          ReleasePanel(vgui::VPANEL panel) = 0;
	virtual void          MovePopupToFront(vgui::VPANEL panel) = 0;
	virtual void          MovePopupToBack(vgui::VPANEL panel) = 0;
	virtual void          SolveTraverse(vgui::VPANEL panel, bool forceApplySchemeSettings = false) = 0;
	virtual void          PaintTraverse(vgui::VPANEL panel) = 0;
	virtual void          EnableMouseCapture(vgui::VPANEL panel, bool state) = 0;
	virtual void          GetWorkspaceBounds(int& x, int& y, int& wide, int& tall) = 0;
	virtual void          GetAbsoluteWindowBounds(int& x, int& y, int& wide, int& tall) = 0;
	virtual void          GetProportionalBase(int& width, int& height) = 0;
	virtual void          CalculateMouseVisible() = 0;
	virtual bool          NeedKBInput() = 0;
	virtual bool          HasCursorPosFunctions() = 0;
	virtual void          SurfaceGetCursorPos(int& x, int& y) = 0;
	virtual void          SurfaceSetCursorPos(int x, int y) = 0;
	virtual void          DrawTexturedLine(const vertex_t& a, const vertex_t& b) = 0;
	virtual void          DrawOutlinedCircle(int x, int y, int radius, int segments) = 0;
	virtual void          DrawTexturedPolyLine(const vertex_t* p, int n) = 0;
	virtual void          DrawTexturedSubRect(int x0, int y0, int x1, int y1, float texs0, float text0, float texs1, float text1) = 0;
	virtual void          DrawTexturedPolygon(int n, vertex_t* pVertice, bool bClipVertices = true) = 0;

	void DrawT(int X, int Y, Color Color, int Font, bool Center, const char* _Input, ...)
	{
		int apple = 0;
		/* set up buffer */
		char Buffer[2048] = { '\0' };

		/* set up varargs*/
		va_list Args;

		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);

		size_t Size = strlen(Buffer) + 1;

		/* set up widebuffer*/
		wchar_t* WideBuffer = new wchar_t[Size];

		/* char -> wchar */
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		/* check center */
		int Width = 0, Height = 0;

		if (Center)
		{
			GetTextSize(Font, WideBuffer, Width, Height);
		}

		/* call and draw*/
		DrawSetTextColor(Color.r, Color.g, Color.b, Color.a);
		DrawSetTextFont(Font);
		DrawSetTextPos(X - (Width / 2), Y);
		DrawPrintText(WideBuffer, wcslen(WideBuffer));

		return;
	}
	void Line(int x, int y, int x2, int y2, Color color)
	{
		DrawSetColor((int)color.r, (int)color.g, (int)color.b, (int)color.a);
		DrawLine(x, y, x2, y2);
	}
	void draw_outlined_rect(int x, int y, int width, int height,int thikness, Color color)
	{
		int x2 = x + width;
		int y2 = y + height;
		DrawSetColor((int)color.r, (int)color.g, (int)color.b, (int)color.a);
		this->DrawFilledRect(x, y, x2, (y + thikness));//top
		this->DrawFilledRect(x, (y2 - thikness), x2,y2);//bottom
		this->DrawFilledRect(x, y, (x + thikness),y2 );//left
		this->DrawFilledRect((x2 - thikness), y, x2,y2 );//right
		DrawSetColor(0, 0, 0, (int)color.a);
		this->DrawOutlinedRect((x +thikness), (y + thikness), (x2 - thikness), (y2 - thikness));
		this->DrawOutlinedRect(x, y, x2, y2);
	}
	void draw_filled_rect(int x, int y, int width, int height, Color color)
	{
		int x2 = x + width;
		int y2 = y + height;
		DrawSetColor((int)color.r, (int)color.g, (int)color.b, (int)color.a);
		this->DrawFilledRect(x, y, x2, y2);
	}
	void draw_rect(int x, int y, int width, int height, Color color)
	{
		int x2 = x + width;
		int y2 = y + height;
		DrawSetColor((int)color.r, (int)color.g, (int)color.b, (int)color.a);
		this->DrawOutlinedRect(x, y, x2, y2);
	}
	std::wstring widen(const std::string& str)
	{
		std::wostringstream wstm;
		const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());
		for (size_t i = 0; i < str.size(); ++i)
			wstm << ctfacet.widen(str[i]);
		return wstm.str();
	}
	void          get_text_size(vgui::HFont font, std::string text, int& wide, int& tall)
	{
		this->GetTextSize(font, widen(text).c_str(), wide, tall);
	}
	void DrawTexturedPoly(int n, vertex_t* vertice, Color col) {
	static int texture_id = this->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	this->DrawSetTextureRGBA(texture_id, buf, 1, 1);
	this->DrawSetColor(col.r, col.g, col.b, col.a);
	this->DrawSetTexture(texture_id);
	this->DrawTexturedPolygon(n, vertice,true);
	}

	void DrawFilledCircle(Vector center, Color color, float radius, float points) {
		std::vector<vertex_t> vertices;
		float step = (float)6.28f  / points;

		for (float a = 0; a < (6.28f); a += step)
			vertices.push_back(vertex_t(vec2_t(radius * cosf(a) + (int)center.x, radius * sinf(a) + (int)center.y)));

		DrawTexturedPoly((int)points, vertices.data(), color);
	}
	void RenderCircle3D(Vector position, float points, float radius, Color color)
	{
		float step = (float)3.14f * 2.0f / points;

		for (float a = 0; a < (3.14f * 2.0f); a += step)
		{
			Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
			Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

			Vector start2d, end2d;
		//	if (g_DebugOverlay->ScreenPosition(start, start2d) || g_DebugOverlay->ScreenPosition(end, end2d))
			//	return;

			//RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
		}
	}

};