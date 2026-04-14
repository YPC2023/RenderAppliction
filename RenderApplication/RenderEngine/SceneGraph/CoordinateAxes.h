#pragma once
#include <string>
#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "SceneComponents.h"

class CoordinateAxes
{
public:
	typedef struct _S_COORDS_DESC
	{
		glm::vec3	start = glm::vec3(0.0f);
		glm::vec3	normal = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec4	color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		float		length = 1.0f;
		float		radius = 0.05f;
	}S_COORDS_DESC;
public:
	CoordinateAxes(const S_COORDS_DESC& desc, const std::string& strName = "");
	~CoordinateAxes();
private:
	entt::entity SetupCoordinateAxes();

public:
	S_COORDS_DESC	m_desc;
	std::string		m_strName;
	entt::entity	m_ModelId;
};