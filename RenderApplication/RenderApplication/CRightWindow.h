#pragma once
#include "CWindow.h"
#include "CEngineBuilder.h"

class CRightWindow : public CWindow
{
	friend class CWindowManager;
protected:
	CRightWindow(const std::string& strName);
public:
	bool Initialize() override;
	void UnInitialize() override;
	void SetSessionInterface(ISessionInterface* pInterface);
public:
	void OnRender() override;
	void OnSize(int x, int y) override;
private:
	void MergeMode();
	void DestroyCamera();
};