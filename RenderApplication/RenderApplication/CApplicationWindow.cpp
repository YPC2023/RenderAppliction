#include "CApplicationWindow.h"


#define IDM_SRC_WIDTH	800
#define IDM_SRC_HEIGHT	600
#define IDM_WINDOW_TITLE	"OpenGL Window"

CApplicationWindow::CApplicationWindow()
{
	m_strTitle = IDM_WINDOW_TITLE;
	m_pIEngineInterface = nullptr;
}

bool CApplicationWindow::Initialize()
{
	if (!InitializeGLFW()) {
		PRINTLOG("Fail to initialize GLFW");
		return false;
	}
	if (!InitializeGLAD()) {
		PRINTLOG("Fail to initialize GLAD");
		return false;
	}
	if (!InitializeEngine()) {
		PRINTLOG("Fail to initiailize Engine");
		return false;
	}
	if (!InitializeWindow()) {
		PRINTLOG("Fail to initialize Windows");
		return false;
	}
	return true;
}

void CApplicationWindow::UnInitialize()
{
	if (nullptr != m_pIEngineInterface) {
		m_pIEngineInterface->UnInitialize();
	}
	ReleaseEngine();
	m_pIEngineInterface = nullptr;
}

bool CApplicationWindow::InitializeEngine()
{
	m_pIEngineInterface = AquireEngine();
	if (nullptr == m_pIEngineInterface) {
		PRINTLOG("Fail to initialize Engine");
		return false;
	}
	if (!m_pIEngineInterface->SetLoader((GLADloadproc)glfwGetProcAddress)) {
		PRINTLOG("Fail to initialize GLAD");
		return false;
	}
	if (!m_pIEngineInterface->Initialize()) {
		PRINTLOG("Fail to initialize Engine");
		return false;
	}
	if (!m_pIEngineInterface->LoadModel(CUtils::GetModelPathFile("cyborg/cyborg.obj").c_str())) {
		PRINTLOG("Fail load mode file(%s)", CUtils::GetModelPathFile("cyborg/cyborg.obj").c_str());
		return false;
	}
	return true;
}

bool CApplicationWindow::InitializeGLFW()
{
	if (!glfwInit()) {
		PRINTLOG("Fail to initialize GLFW");
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	m_pMainWindow = glfwCreateWindow(IDM_SRC_WIDTH, IDM_SRC_HEIGHT, m_strTitle.c_str(), nullptr, nullptr);
	if (m_pMainWindow == nullptr) {
		PRINTLOG("Fail to create window");
		return false;
	}
	glfwMakeContextCurrent(m_pMainWindow);
	return true;
}

bool CApplicationWindow::InitializeGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		PRINTLOG("Failed to initialize GLAD");
		return false;
	}
	return true;
}

void CApplicationWindow::SetImguiStyle()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
}

bool CApplicationWindow::InitializeWindow()
{
	m_pWindowLeft = CWindowManager::GetInstance().CreateWindow2("Left");
	if (0 == m_pWindowLeft) {
		PRINTLOG("Fail to create [%s] Window", m_pWindowLeft->GetName().c_str());
		return false;
	}
	if (!m_pWindowLeft->Initialize()) {
		PRINTLOG("Fail to initialize [%s] Window", m_pWindowLeft->GetName().c_str());
		return false;
	}

	m_pWindowMiddleTop = CWindowManager::GetInstance().CreateWindow2("RenderWindow", CWindowManager::E_WINDOW_RENDER);
	if (0 == m_pWindowMiddleTop) {
		PRINTLOG("Fail to create [%s] Window", m_pWindowMiddleTop->GetName().c_str());
		return false;
	}
	if (!m_pWindowMiddleTop->Initialize()) {
		PRINTLOG("Fail to initialize [%s] Window", m_pWindowMiddleTop->GetName().c_str());
		return false;
	}

	m_pWindowMiddleBottom = CWindowManager::GetInstance().CreateWindow2("MiddleBottom", CWindowManager::E_WINDOW_RENDER);
	if (0 == m_pWindowMiddleBottom) {
		PRINTLOG("Fail to create [%s] Window", m_pWindowMiddleBottom->GetName().c_str());
		return false;
	}
	if (!m_pWindowMiddleBottom->Initialize()) {
		PRINTLOG("Fail to initialize [%s] Window", m_pWindowMiddleBottom->GetName().c_str());
		return false;
	}

	m_pWindowRight = CWindowManager::GetInstance().CreateWindow2("Property", CWindowManager::E_WINDOW_PROPERTY);
	if (0 == m_pWindowRight) {
		PRINTLOG("Fail to create [%s] Window", m_pWindowRight->GetName().c_str());
		return false;
	}
	if (!m_pWindowRight->Initialize()) {
		PRINTLOG("Fail to initialize [%s] Window", m_pWindowRight->GetName().c_str());
		return false;
	}
	// 公用会话接口
	m_pWindowRight->SetSessionInterface(m_pWindowMiddleTop->GetSessionInterface());
	return true;
}

void CApplicationWindow::SetTitle(const std::string& strTitle)
{
	m_strTitle = strTitle;
}

void CApplicationWindow::Run()
{
	SetImguiStyle();
	ImGui_ImplGlfw_InitForOpenGL(m_pMainWindow, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	while (!glfwWindowShouldClose(m_pMainWindow))
	{
		glfwPollEvents();
		glfwSwapBuffers(m_pMainWindow);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

		m_pWindowLeft->Render();
		m_pWindowMiddleTop->Render();
		m_pWindowMiddleBottom->Render();
		m_pWindowRight->Render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_pMainWindow);
	glfwTerminate();
}