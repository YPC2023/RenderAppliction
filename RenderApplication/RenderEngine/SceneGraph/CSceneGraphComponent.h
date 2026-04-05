#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Resource/CTexture.h"
#include "../Core/Define.h"

class CSceneGraphComponent
{
public:
	struct S_TEXTURE_INFO
	{
		std::shared_ptr<CTexture>	texture;
		std::string					strType = "";
		std::string					strName = "";
	};
	struct S_MESH_INFO
	{
		unsigned int                Type = 0;
		HVAO						VAO = 0;
		HVBO		                VBO = 0;
		HEBO		                EBO = 0;
		size_t						size = 0;
		std::vector<S_TEXTURE_INFO>	textures;
	};
	struct S_MODEL_INFO
	{
		std::string					strName = "";
	};
	struct S_TRANSFORM_INFO
	{
		glm::vec3				translation = glm::vec3(0.0f);
		glm::quat				rotation = glm::vec3(0.0f);
		glm::vec3				scale = glm::vec3(1.0f);
		bool					dirty = true;
		glm::mat4				matrix = glm::mat4(1.0f);
		
		void update(const glm::mat4& base) {
			matrix = glm::translate(base, translation);
			matrix = matrix * glm::mat4_cast(rotation);
			matrix = glm::scale(matrix, scale);
		}
	};
	struct S_RELATION_INFO
	{
		entt::entity			parent = entt::null;
		std::set<entt::entity>	children;
	};

};