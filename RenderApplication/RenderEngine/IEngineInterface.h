#pragma once

#include <glad/glad.h>

// 这里的 MYLIB_EXPORTS 是我们在 DLL 项目属性中定义的开关宏
#ifdef ENGINE_API_EXPORTS
	// 如果定义了 MYLIB_EXPORTS，说明正在编译 DLL，执行导出
#define ENGINE_API_ACTION __declspec(dllexport)
#else
	// 如果没有定义，说明是外部项目在使用该库，执行导入
#define ENGINE_API_ACTION __declspec(dllimport)
#endif

class ENGINE_API_ACTION IEngineInterface
{
public:
	virtual bool Initialize() = 0;
	virtual void UnInitialize() = 0;
	virtual bool SetLoader(GLADloadproc loader) = 0;
	virtual bool LoadModel(const char* path) = 0;
};

class ENGINE_API_ACTION ISessionInterface
{
public:
	typedef enum _E_MOUSE_ACTION_TYPE
	{
		E_MOUSE_ACTION_UNKOWN,
		E_MOUSE_ACTION_PRESS,
		E_MOUSE_ACTION_RELEASE,
		E_MOUSE_ACTION_MOVING,
		E_MOUSE_ACTION_WHEEL
	}E_MOUSE_ACTION_TYPE;
	typedef enum _E_MOUSE_BUTTON_TYPE
	{
		E_MOUSE_BUTTON_UNKOWN,
		E_MOUSE_BUTTON_LEFT,
		E_MOUSE_BUTTON_RIGHT,
		E_MOUSE_BUTTON_MIDDLE
	}E_MOUSE_BUTTON_TYPE;
public:
	virtual bool Initialize() = 0;
	virtual void UnInitialize() = 0;
	virtual void Render() = 0;
	virtual unsigned int GetRenderTextureId() = 0;
	virtual void Resize(int width, int height) = 0;
public:
	virtual void OnMouseAction(E_MOUSE_BUTTON_TYPE key, E_MOUSE_ACTION_TYPE action, int x, int y) = 0;
};