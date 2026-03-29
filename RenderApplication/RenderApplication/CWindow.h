#pragma once

#include "CUtils.h"
#include "IWindowInterface.h"
#include "imgui.h"

class CWindow : public IWindowInterface
{
	friend class CWindowManager;
protected:
	CWindow(const std::string& strName);
public:
	bool Initialize() override;
	void UnInitialize() override;
	void OnRender() override;
	void OnSize(int x, int y) override;
	void OnMouseAction(E_MOUSE_BUTTON_TYPE key, E_MOUSE_ACTION_TYPE action, int x, int y) override;
	void OnKeyboardAction(unsigned int key, const char* name, bool down) override;
public:
	void Render();
	const std::string& GetName() { return m_strName; }
private:
	void RenderBegin();
	void RenderEnd();
	void RenderDebugPoint();
private:
	bool IsMouseKey(ImGuiKey key);
	void CheckSizeAction();
	void CheckMouseAction();
	void CheckKeyboardAction();
	E_MOUSE_BUTTON_TYPE TranslateImguiToLocalButton(ImGuiMouseButton_ key);
protected:
	std::string		m_strName;
	ImVec2			m_LastSize;
	uint			m_ID;
protected:
	ImVec2			m_Debug_Point;
	bool			m_Debug_Enable;
};

