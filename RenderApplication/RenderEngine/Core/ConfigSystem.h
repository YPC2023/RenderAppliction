#pragma once
#include <glm/glm.hpp>
#include <string>

class ConfigSystem
{
public:
	static int GetViewportWidth();
	static int GetViewportHeight();
	static glm::vec3 GetCameraPos();
	static glm::vec3 GetCameraTarget();
	static glm::vec3 GetCameraUp();
	static float GetFov();
	static float GetNear();
	static float GetFar();
	static std::string GetSulotionDirPath();
	static std::string GetGlslTemplateDirPath();
	static std::string GetDefaultVertexTemplateFilePath();
	static std::string GetDefaultFragmentTemplateFilePath();
};