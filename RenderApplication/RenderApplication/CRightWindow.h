#pragma once
#include "CWindow.h"

class CRightWindow : public CWindow
{
	friend class CWindowManager;
protected:
	CRightWindow(const std::string& strName);
public:
	bool Initialize() override;
	void UnInitialize() override;
public:
	void OnRender() override;
	void OnSize(int x, int y) override;
};