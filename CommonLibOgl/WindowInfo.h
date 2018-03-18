#pragma once


namespace CommonLibOgl
{
	// Information about the window.
	struct WindowInfo
	{
	public:
		// Note: to create a window without menu, omit the menuId argument.
		WindowInfo(int clientWidth, int clientHeight, UINT titleId, UINT wndClassId, WORD iconId, WORD iconSmallId, WORD menuId = 0)
			: m_clientWidth(clientWidth), m_clientHeight(clientHeight), m_titleId(titleId), 
			  m_wndClassId(wndClassId), m_iconId(iconId), m_iconSmallId(iconSmallId), m_menuId(menuId)
		{
		}

	public:
		// Client area width and height (in pixels).
		int m_clientWidth;
		int m_clientHeight;

		// Window title ID.
		UINT m_titleId;

		// Window class ID.
		UINT m_wndClassId;

		// Window icon ID.
		WORD m_iconId;

		// Window small icon ID.
		WORD m_iconSmallId;

		// Window menu ID.
		WORD m_menuId;
	};
}
