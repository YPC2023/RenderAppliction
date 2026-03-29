#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "CUtils.h"
#include "InitializeInterface.h"
#include "ISingletonInterface.h"
#include "CWindowManager.h"
#include "IEngineInterface.h"


class CApplicationWindow : 
	public InitializeInterface, 
	public ISingletonInterface<CApplicationWindow>
{
	// 必须让基类能访问子类的私有构造函数
	friend class ISingletonInterface<CApplicationWindow>;
private:
	CApplicationWindow();
public:
	bool Initialize() override;
	void UnInitialize() override;
private:
	bool InitializeEngine();
	bool InitializeGLFW();
	bool InitializeGLAD();
	void SetImguiStyle();
	bool InitializeWindow();
public:
	void SetTitle(const std::string& strTitle);
	const std::string& GetTitle() { return m_strTitle; }
public:
	void Run();
private:
	GLFWwindow* m_pMainWindow;
	std::string m_strTitle;
private:
	CWindow* m_pWindowLeft;
	CWindow* m_pWindowMiddleTop;
	CWindow* m_pWindowMiddleBottom;
	CWindow* m_pWindowRight;

private:
	IEngineInterface*	m_pIEngineInterface;

};

