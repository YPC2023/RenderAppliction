#include "CWindowManager.h"
#include "CRightWindow.h"

CWindow* CWindowManager::CreateWindow2(const std::string& strName, E_WINDOW_TYPE type)
{
	if (0 < m_map_Window.count(strName)) {
		return m_map_Window[strName];
	}
	CWindow* pWindow = 0;
	if (E_WINDOW_RENDER == type) {
		pWindow = new CRenderWindow(strName);
	}
	else if (E_WINDOW_PROPERTY == type) {
		pWindow = new CRightWindow(strName);
	}
	else {
		pWindow = new CWindow(strName);
	}
	m_map_Window[strName] = pWindow;
	if (0 == pWindow) {
		PRINTLOG("Fail to new Window");
		return 0;
	}
	return pWindow;
}
