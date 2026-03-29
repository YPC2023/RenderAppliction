#pragma once

#include <imgui.h>
#include "CWindow.h"
#include "CEngineBuilder.h"

class CRenderWindow : public CWindow
{
	friend class CWindowManager;
protected:
	CRenderWindow(const std::string& strName);
public:
	bool Initialize() override;
	void UnInitialize() override;
public:
	void OnRender() override;
	void OnSize(int x, int y) override;
	void OnMouseAction(E_MOUSE_BUTTON_TYPE key, E_MOUSE_ACTION_TYPE action, int x, int y) override;
	void OnKeyboardAction(unsigned int key, const char* name, bool down) override;
private:
	ImVec2	GetRelativePosition(const ImVec2& point);
	IEngineInterface::E_MOUSE_BUTTON_TYPE TranslateImguiButtonToEngine(E_MOUSE_BUTTON_TYPE key);
	IEngineInterface::E_MOUSE_ACTION_TYPE TranslateImguiActionToEngine(E_MOUSE_ACTION_TYPE action);
private:
	IEngineInterface* m_pEngineInterface;
};

