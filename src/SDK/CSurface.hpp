#ifndef SKD_CSURFACE_HPP
#define SKD_CSURFACE_HPP

#include "Color.hpp"
#include "IAppSystem.hpp"
#include "IEngineVGui.hpp"


class IImage;
class Image;
class IHTML;
class IHTMLEvents;
class IHTMLResponses;
class IHTMLChromeController;

typedef unsigned long HCursor;
typedef unsigned long HTexture;
typedef unsigned long HFont;

class CSurface : public IBaseInterface, public IAppSystem
{
public:
	virtual ~CSurface() = 0;

	virtual void Shutdown() = 0;

	virtual void ApplyChanges() = 0;

	// hierarchy root
	virtual VPANEL GetEmbeddedPanel() = 0;
	virtual void SetEmbeddedPanel(VPANEL pPanel) = 0;

	// drawing context
	virtual void PushMakeCurrent(VPANEL panel, bool useInsets) = 0;
	virtual void PopMakeCurrent(VPANEL panel) = 0;

	// rendering functions
	virtual void DrawSetColor(int r, int g, int b, int a) = 0;
	virtual void DrawSetColor(Color col) = 0;

	virtual void DrawFilledRect(int x0, int y0, int x1, int y1) = 0;
	virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;

	virtual void DrawLine(int x0, int y0, int x1, int y1) = 0;
	virtual void DrawPolyLine(int *px, int *py, int numPoints) = 0;

	virtual void DrawSetTextFont(HFont font) = 0;
	virtual void DrawSetTextColor(int r, int g, int b, int a) = 0;
	virtual void DrawSetTextColor(Color col) = 0;
	virtual void DrawSetTextPos(int x, int y) = 0;
	virtual void DrawGetTextPos(int &x, int &y) = 0;
	virtual void DrawPrintText(const wchar_t *text, int textLen) = 0;
	virtual void DrawUnicodeChar(wchar_t wch) = 0;
	virtual void DrawUnicodeCharAdd(wchar_t wch) = 0;

	virtual void DrawFlushText() = 0; // flushes any buffered text (for rendering optimizations)
	virtual IHTML *CreateHTMLWindow(IHTMLEvents *events, VPANEL context) = 0;
	virtual void PaintHTMLWindow(IHTML *htmlwin) = 0;
	virtual void DeleteHTMLWindow(IHTML *htmlwin) = 0;

	virtual void DrawSetTextureFile(int id, const char *filename, int hardwareFilter, bool forceReload) = 0;
	virtual void DrawSetTextureRGBA(int id, const unsigned char *rgba, int wide, int tall, int hardwareFilter, bool forceReload) = 0;
	virtual void DrawSetTexture(int id) = 0;
	virtual void DrawGetTextureSize(int id, int &wide, int &tall) = 0;
	virtual void DrawTexturedRect(int x0, int y0, int x1, int y1) = 0;
	virtual bool IsTextureIDValid(int id) = 0;

	virtual int CreateNewTextureID(bool procedural = false) = 0;

	virtual void GetScreenSize(int &wide, int &tall) = 0;
	virtual void SetAsTopMost(VPANEL panel, bool state) = 0;
	virtual void BringToFront(VPANEL panel) = 0;
	virtual void SetForegroundWindow (VPANEL panel) = 0;
	virtual void SetPanelVisible(VPANEL panel, bool state) = 0;
	virtual void SetMinimized(VPANEL panel, bool state) = 0;
	virtual bool IsMinimized(VPANEL panel) = 0;
	virtual void FlashWindow(VPANEL panel, bool state) = 0;
	virtual void SetTitle(VPANEL panel, const wchar_t *title) = 0;
	virtual void SetAsToolBar(VPANEL panel, bool state) = 0;		// removes the window's task bar entry (for context menu's, etc.)

	// windows stuff
	virtual void CreatePopup(VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true, bool kbInput = true) = 0;
	virtual void SwapBuffers(VPANEL panel) = 0;
	virtual void Invalidate(VPANEL panel) = 0;
	virtual void SetCursor(HCursor cursor) = 0;
	virtual bool IsCursorVisible() = 0;
	virtual bool IsWithin(int x, int y) = 0;
	virtual bool HasFocus() = 0;

	// returns true if the surface supports minimize & maximize capabilities
	MAKE_CENUMV_Q(SurfaceFeature_e, int,
		ANTIALIASED_FONTS, 1,
		DROPSHADOW_FONTS, 2,
		ESCAPE_KEY, 3,
		OPENING_NEW_HTML_WINDOWS, 4,
		FRAME_MINIMIZE_MAXIMIZE, 5,
		DIRECT_HWND_RENDER, 6,
	);

	virtual bool SupportsFeature(SurfaceFeature_e feature) = 0;

	// restricts what gets drawn to one panel and it's children
	// currently only works in the game
	virtual void RestrictPaintToSinglePanel(VPANEL panel) = 0;

	// these two functions obselete, use IInput::SetAppModalSurface() instead
	virtual void SetModalPanel(VPANEL) = 0;
	virtual VPANEL GetModalPanel() = 0;

	virtual void UnlockCursor() = 0;
	virtual void LockCursor() = 0;
	virtual void SetTranslateExtendedKeys(bool state) = 0;
	virtual VPANEL GetTopmostPopup() = 0;

	// engine-only focus handling (replacing WM_FOCUS windows handling)
	virtual void SetTopLevelFocus(VPANEL panel) = 0;

	// fonts
	// creates an empty handle to a vgui font.  windows fonts can be add to this via SetFontGlyphSet().
	virtual HFont CreateFont() = 0;

	// adds to the font
	MAKE_CENUMV_Q(EFontFlags, int,
		FONTFLAG_NONE,         0x000,
		FONTFLAG_ITALIC,       0x001,
		FONTFLAG_UNDERLINE,    0x002,
		FONTFLAG_STRIKEOUT,    0x004,
		FONTFLAG_SYMBOL,       0x008,
		FONTFLAG_ANTIALIAS,    0x010,
		FONTFLAG_GAUSSIANBLUR, 0x020,
		FONTFLAG_ROTARY,       0x040,
		FONTFLAG_DROPSHADOW,   0x080
	);

	virtual bool AddGlyphSetToFont(HFont font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int lowRange, int highRange) = 0;

	// adds a custom font file (only supports true type font files (.ttf) for now)
	virtual bool AddCustomFontFile(const char *fontFileName) = 0;

	// returns the details about the font
	virtual int GetFontTall(HFont font) = 0;
	virtual void GetCharABCwide(HFont font, int ch, int &a, int &b, int &c) = 0;
	virtual int GetCharacterWidth(HFont font, int ch) = 0;
	virtual void GetTextSize(HFont font, const wchar_t *text, int &wide, int &tall) = 0;

	// notify icons?!?
	virtual VPANEL GetNotifyPanel() = 0;
	virtual void SetNotifyIcon(VPANEL context, HTexture icon, VPANEL panelToReceiveMessages, const char *text) = 0;

	// plays a sound
	virtual void PlaySound(const char *fileName) = 0;

	//!! these functions should not be accessed directly, but only through other vgui items
	//!! need to move these to seperate interface
	virtual int GetPopupCount() = 0;
	virtual VPANEL GetPopup(int index) = 0;
	virtual bool ShouldPaintChildPanel(VPANEL childPanel) = 0;
	virtual bool RecreateContext(VPANEL panel) = 0;
	virtual void AddPanel(VPANEL panel) = 0;
	virtual void ReleasePanel(VPANEL panel) = 0;
	virtual void MovePopupToFront(VPANEL panel) = 0;
	virtual void MovePopupToBack(VPANEL panel) = 0;

	virtual void SolveTraverse(VPANEL panel, bool forceApplySchemeSettings = false) = 0;
	virtual void PaintTraverse(VPANEL panel) = 0;

	virtual void EnableMouseCapture(VPANEL panel, bool state) = 0;

	// returns the size of the workspace
	virtual void GetWorkspaceBounds(int &x, int &y, int &wide, int &tall) = 0;

	// gets the absolute coordinates of the screen (in windows space)
	virtual void GetAbsoluteWindowBounds(int &x, int &y, int &wide, int &tall) = 0;

	// gets the base resolution used in proportional mode
	virtual void GetProportionalBase(int &width, int &height) = 0;

	virtual void CalculateMouseVisible() = 0;
	virtual bool NeedKBInput() = 0;

	virtual bool HasCursorPosFunctions() = 0;
	virtual void SurfaceGetCursorPos(int &x, int &y) = 0;
	virtual void SurfaceSetCursorPos(int x, int y) = 0;

 	// SRC only functions!!!
	virtual void DrawTexturedPolygon(struct VGuiVertex *pVertices, int n) = 0;
	virtual int GetFontAscent(HFont font, wchar_t wch) = 0;

	// web browser
	virtual void SetAllowHTMLJavaScript(bool state) = 0;
	virtual void SetLanguage(const char *language) = 0;
	virtual const char *GetLanguage() = 0;
	virtual bool DeleteTextureByID(int id) = 0;
	virtual void DrawUpdateRegionTextureBGRA(int nTextureID, int x, int y, const unsigned char *pchData, int wide, int tall) = 0;
	virtual void DrawSetTextureBGRA(int id, const unsigned char *rgba, int wide, int tall) = 0;
	virtual void CreateBrowser(VPANEL panel, IHTMLResponses *pBrowser, bool bPopupWindow, const char *pchUserAgentIdentifier) = 0;
	virtual void RemoveBrowser(VPANEL panel, IHTMLResponses *pBrowser) = 0;
	virtual IHTMLChromeController *AccessChromeHTMLController() = 0;
};

#endif // SKD_CSURFACE_HPP
