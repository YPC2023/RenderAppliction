#include "CRenderWindow.h"
//#include "CEngineManager.h"
#include <GLFW/glfw3.h>


CRenderWindow::CRenderWindow(const std::string& strName): CWindow(strName)
{
	m_pEngineInterface = nullptr;
}

bool CRenderWindow::Initialize()
{
	m_pEngineInterface = CEngineBuilder::CreateEngineSession();
	if (nullptr == m_pEngineInterface) {
		PRINTLOG("Fail to create engine session");
		return false;
	}
	if (!m_pEngineInterface->SetLoader((GLADloadproc)glfwGetProcAddress)) {
		PRINTLOG("Fail to initialize GLAD");
	}
	if (!m_pEngineInterface->Initialize()) {
		PRINTLOG("Fail to initialize Engine");
		return false;
	}
	return true;
}

void CRenderWindow::UnInitialize()
{
	if (nullptr != m_pEngineInterface) {
		m_pEngineInterface->UnInitialize();
	}
}

void CRenderWindow::OnRender()
{
	if (nullptr == m_pEngineInterface) {
		return;
	}
	m_pEngineInterface->Render();
	ImTextureID texID = (ImTextureID)m_pEngineInterface->GetRenderTextureId();
	if (0 == texID) {
		return;
	}
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	// 注意：OpenGL 坐标系 y 轴向上，ImGui 向下，可能需要翻转 UV
	ImGui::Image(texID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
}

void CRenderWindow::OnSize(int x, int y)
{
	if (nullptr != m_pEngineInterface) {
		//PRINTLOG("%s(%d,%d)",m_strName.c_str(), x, y);
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		m_pEngineInterface->Resize((int)viewportSize.x, (int)viewportSize.y);
	}
}

void CRenderWindow::OnMouseAction(E_MOUSE_BUTTON_TYPE key, E_MOUSE_ACTION_TYPE action, int x, int y)
{
	if (nullptr != m_pEngineInterface) 
	{
		if (E_MOUSE_ACTION_WHEEL == action) {
			m_pEngineInterface->OnMouseAction(TranslateImguiButtonToEngine(key), 
				TranslateImguiActionToEngine(action), x, y);
		}
		else {
			ImVec2 relativePos = GetRelativePosition(ImGui::GetIO().MousePos);
			m_pEngineInterface->OnMouseAction(TranslateImguiButtonToEngine(key),
				TranslateImguiActionToEngine(action), (int)relativePos.x, (int)relativePos.y);
		}
	}
}

void CRenderWindow::OnKeyboardAction(unsigned int key, const char* name, bool down)
{
	//PRINTLOG("%s[%s(%d):%s]", m_strName.c_str(), name, key, down ? "DOWN" : "UP");
	//CEngineManager::GetInstance().GetEngine().KeyboardAction(key, name, down);
}

ImVec2	CRenderWindow::GetRelativePosition(const ImVec2& point)
{
	// 1. 获取当前内容区域的起始屏幕坐标 (左上角)
	// 注意：如果是紧接着 UI 元素后渲染，这个坐标会随着布局下移
	ImVec2 vMin = ImGui::GetCursorScreenPos();
	// 2. 计算相对坐标
	return ImVec2(point.x - vMin.x, point.y - vMin.y);
}

IEngineInterface::E_MOUSE_BUTTON_TYPE CRenderWindow::TranslateImguiButtonToEngine(E_MOUSE_BUTTON_TYPE key)
{
	switch (key) {
	case E_MOUSE_BUTTON_LEFT:
		return IEngineInterface::E_MOUSE_BUTTON_LEFT;
	case E_MOUSE_BUTTON_RIGHT:
		return IEngineInterface::E_MOUSE_BUTTON_RIGHT;
	case E_MOUSE_BUTTON_MIDDLE:
		return IEngineInterface::E_MOUSE_BUTTON_MIDDLE;
	}
	return IEngineInterface::E_MOUSE_BUTTON_UNKOWN;
}

IEngineInterface::E_MOUSE_ACTION_TYPE CRenderWindow::TranslateImguiActionToEngine(E_MOUSE_ACTION_TYPE action)
{
	switch (action) {
	case E_MOUSE_ACTION_PRESS:
		return IEngineInterface::E_MOUSE_ACTION_PRESS;
	case E_MOUSE_ACTION_RELEASE:
		return IEngineInterface::E_MOUSE_ACTION_RELEASE;
	case E_MOUSE_ACTION_MOVING:
		return IEngineInterface::E_MOUSE_ACTION_MOVING;
	case E_MOUSE_ACTION_WHEEL:
		return IEngineInterface::E_MOUSE_ACTION_WHEEL;
	}
	return IEngineInterface::E_MOUSE_ACTION_UNKOWN;
}
