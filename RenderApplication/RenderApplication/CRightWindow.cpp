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
		MergeMode();
	}
}

void CRightWindow::OnSize(int x, int y)
{

}

void CRightWindow::MergeMode()
{
	if (nullptr == m_pISessionInterface) {
		PRINTLOG("Initialize session interface first. please");
		return;
	}

	if (!m_pISessionInterface->MergeModel()) {
		PRINTLOG("Fail to merge model");
	}
}