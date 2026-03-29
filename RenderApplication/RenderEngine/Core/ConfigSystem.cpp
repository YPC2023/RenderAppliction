#include "ConfigSystem.h"
#include "Define.h"
#include <CUtils.h>

int ConfigSystem::GetViewportWidth()
{
	return IDM_VIEWPORT_WIDTH;
}

int ConfigSystem::GetViewportHeight()
{
	return IDM_VIEWPORT_HEIGHT;
}

glm::vec3 ConfigSystem::GetCameraPos()
{
	return CONST_CAMERA_POS;
}

glm::vec3 ConfigSystem::GetCameraTarget()
{
	return CONST_CAMERA_TARGET;
}

glm::vec3 ConfigSystem::GetCameraUp()
{
	return CONST_CAMERA_UP;
}

float ConfigSystem::GetFov()
{
	return CONST_CAMERA_FOV;
}

float ConfigSystem::GetNear()
{
	return CONST_CAMERA_NEAR;
}

float ConfigSystem::GetFar()
{
	return CONST_CAMERA_FAR;
}

std::string ConfigSystem::GetSulotionDirPath()
{
	return CUtils::GetSolutionPathDir();
}

std::string ConfigSystem::GetGlslTemplateDirPath()
{
	return GetSulotionDirPath() + "RenderEngine/Material/glsl/";
}

std::string ConfigSystem::GetDefaultVertexTemplateFilePath()
{
	return GetGlslTemplateDirPath() + "template.vs";
}

std::string ConfigSystem::GetDefaultFragmentTemplateFilePath()
{
	return GetGlslTemplateDirPath() + "template.fs";
}
