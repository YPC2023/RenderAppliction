#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>


class CoordinateArc
{
public:
	typedef struct _S_COORDS_DESC
	{
		glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 refVector = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec4 sColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 eColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		float startAngle = 0.0f;
		float endAngle = 90.0f;
		float outerRadius = 3.0f;
		float ringWidth = 0.1f;
	}S_COORDS_DESC;
public:
	CoordinateArc(const S_COORDS_DESC& desc, const std::string& strName = "");
	~CoordinateArc();

private:
	entt::entity SetupCoordinateArc();


public:
	S_COORDS_DESC	m_desc;
	std::string		m_strName;
	entt::entity	m_ModelId;
};