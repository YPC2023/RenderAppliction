#pragma once

#include "ISingletonInterface.h"
#include "CWindow.h"
#include "CRenderWindow.h"
#include "CUtils.h"
#include <map>

class CWindowManager : public ISingletonInterface<CWindowManager>
{
	// 必须让基类能访问子类的私有构造函数
	friend class ISingletonInterface<CWindowManager>;
public:
	typedef enum _E_WINDOW_TYPE
	{
		E_WINDOW_NORMAL,
		E_WINDOW_RENDER,
	}E_WINDOW_TYPE;
public:
	CWindow* CreateWindow2(const std::string& strName, E_WINDOW_TYPE type = E_WINDOW_NORMAL);
private:
	std::map<std::string, CWindow*>	m_map_Window;
};

