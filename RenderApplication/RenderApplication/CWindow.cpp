#include "CWindow.h"

CWindow::CWindow(const std::string& strName)
{
	static uint index = 0;
	m_ID = ++index;
	m_strName = strName;
	m_LastSize = ImVec2(0, 0);

	m_Debug_Enable = false;
}

bool CWindow::Initialize()
{
	return true;
}

void CWindow::UnInitialize()
{

}

void CWindow::OnRender()
{

}

void CWindow::OnSize(int x, int y)
{
	//PRINTLOG("%s(%d,%d)",m_strName.c_str(), x, y);
}

void CWindow::OnMouseAction(E_MOUSE_BUTTON_TYPE key, E_MOUSE_ACTION_TYPE action, int x, int y)
{
	//PRINTLOG("%s[%d:%s](%d,%d)", m_strName.c_str(), key, down ? "DOWN" : "UP", x, y);
}

void CWindow::OnKeyboardAction(unsigned int key, const char* name, bool down)
{
	//PRINTLOG("%s[%s(%d):%s]", m_strName.c_str(), name, key, down ? "DOWN" : "UP");
}

void CWindow::Render()
{
	RenderBegin();

	CheckSizeAction();
	CheckMouseAction();
	CheckKeyboardAction();

	OnRender();

	RenderDebugPoint();
	RenderEnd();
}

void CWindow::RenderBegin()
{
	ImGui::Begin(m_strName.c_str());
}

void CWindow::RenderEnd()
{
	ImGui::End();
}

void CWindow::RenderDebugPoint()
{
	if (m_Debug_Enable) {
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddCircleFilled(m_Debug_Point, 5.0f, IM_COL32(255, 0, 0, 255));
	}
}

bool CWindow::IsMouseKey(ImGuiKey key)
{
	for (uint index = ImGuiKey_MouseLeft; index <= ImGuiKey_MouseWheelY; ++index) {
		if (key == index) {
			return true;
		}
	}
	return false;
}

void CWindow::CheckSizeAction()
{
	ImVec2 curSize = ImGui::GetWindowSize();
	// 检测尺寸是否发生变化
	if (curSize.x != m_LastSize.x || curSize.y != m_LastSize.y) {
		// 触发尺寸变化逻辑
		OnSize((int)curSize.x, (int)curSize.y);
		// 更新记录
		m_LastSize = curSize;
	}
}

void CWindow::CheckMouseAction()
{
	static ImVec2 lastPosition(0.0f, 0.0f);
	if (!ImGui::IsWindowHovered()) {
		return;
	}
	ImVec2 curPosition = ImGui::GetIO().MousePos;
	uint pressKey = ImGuiMouseButton_COUNT;
	for (uint key = ImGuiMouseButton_Left; key < ImGuiMouseButton_COUNT; ++key) {
		if (ImGui::IsMouseClicked(key)) {
			OnMouseAction(TranslateImguiToLocalButton((ImGuiMouseButton_)key), 
				E_MOUSE_ACTION_PRESS, (int)curPosition.x, (int)curPosition.y);
		}
		else if (ImGui::IsMouseReleased(key)) {
			OnMouseAction(TranslateImguiToLocalButton((ImGuiMouseButton_)key), 
				E_MOUSE_ACTION_RELEASE, (int)curPosition.x, (int)curPosition.y);
		}
		if (ImGui::IsMouseDown(key)) {
			pressKey = key;
		}
	}
	int dx = (int)(curPosition.x - lastPosition.x);
	int dy = (int)(curPosition.y - lastPosition.y);
	if (0 != dx || 0 != dy) {
		OnMouseAction(TranslateImguiToLocalButton((ImGuiMouseButton_)pressKey), 
			E_MOUSE_ACTION_MOVING, (int)curPosition.x, (int)curPosition.y);
		lastPosition = curPosition;
	}
	// 检测滚轮
	float wheel = ImGui::GetIO().MouseWheel;
	if (wheel != 0.0f) {
		OnMouseAction(TranslateImguiToLocalButton((ImGuiMouseButton_)pressKey), 
			E_MOUSE_ACTION_WHEEL, (int)(wheel * 100), (int)curPosition.y);
	}
}

void CWindow::CheckKeyboardAction()
{
	if (!ImGui::IsWindowHovered()) {
		return;
	}
	// 遍历 ImGui 支持的所有键位（从 NamedKey_BEGIN 到 NamedKey_END）
	for (uint key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key)
	{
		if (IsMouseKey((ImGuiKey)key)) {
			continue;
		}
		const char* key_name = ImGui::GetKeyName((ImGuiKey)key);
		if (ImGui::IsKeyPressed((ImGuiKey)key)) {
			// 这一帧刚按下的键
			OnKeyboardAction((uint)key, key_name, true);
		}
		if (ImGui::IsKeyReleased((ImGuiKey)key)) {
			OnKeyboardAction((uint)key, key_name, false);
		}
	}
}

IWindowInterface::E_MOUSE_BUTTON_TYPE CWindow::TranslateImguiToLocalButton(ImGuiMouseButton_ key)
{
	switch (key) {
	case ImGuiMouseButton_Left:
		return E_MOUSE_BUTTON_LEFT;
	case ImGuiMouseButton_Right:
		return E_MOUSE_BUTTON_RIGHT;
	case ImGuiMouseButton_Middle:
		return E_MOUSE_BUTTON_MIDDLE;
	}
	return E_MOUSE_BUTTON_UNKOWN;
}