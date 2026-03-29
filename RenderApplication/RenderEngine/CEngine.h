#pragma once
#include <ISingletonInterface.h>
#include "IEngineInterface.h"

class CEngine : public IEngineInterface,
	public ISingletonInterface<CEngine>
{
public:
	bool Initialize() override;
	void UnInitialize() override;
	bool SetLoader(GLADloadproc loader) override;
	void Render() override;
	unsigned int GetRenderTextureId() override;
	void Resize(int width, int height) override;
	bool LoadModel(const char* path) override;
public:
	void OnMouseAction(E_MOUSE_BUTTON_TYPE key, E_MOUSE_ACTION_TYPE action, int x, int y) override;
};