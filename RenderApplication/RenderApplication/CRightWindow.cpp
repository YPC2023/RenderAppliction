#include "CRightWindow.h"

CRightWindow::CRightWindow(const std::string& strName) : CWindow(strName)
{

}

bool CRightWindow::Initialize()
{
	return true;
}

void CRightWindow::UnInitialize()
{

}

void CRightWindow::SetSessionInterface(ISessionInterface* pInterface)
{
	m_pISessionInterface = pInterface;
}

void CRightWindow::OnRender()
{
	if (ImGui::Button("Merge")) {
		PRINTLOG("OK");
	}
}

void CRightWindow::OnSize(int x, int y)
{

}
