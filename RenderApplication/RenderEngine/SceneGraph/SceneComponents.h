#pragma once
#include <string>
#include <vector>
#include <set>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Core/VarWatcher.h"
#include "../Material/CMaterialSystem.h"

namespace SGCmpnt
{
	struct S_CMPNT_MATERIAL
	{
		std::shared_ptr<CMaterial>	m_Material;
	};
	struct S_CMPNT_MESH
	{
		unsigned int                Type = 0;
		HVAO						VAO = 0;
		HVBO		                VBO = 0;
		HEBO		                EBO = 0;
		size_t						size = 0;
	};
	struct S_CMPNT_TRANSFORM_DATA
	{
		VarWatcher<glm::vec3>	translation = VarWatcher<glm::vec3>(glm::vec3(0.0f));
		VarWatcher<glm::quat>	rotation = VarWatcher<glm::quat>(glm::vec3(0.0f));
		VarWatcher<glm::vec3>	scale = VarWatcher<glm::vec3>(glm::vec3(1.0f));
		bool					dirty = true;
		VarWatcher<glm::mat4>	matrix = VarWatcher<glm::mat4>(glm::mat4(1.0f));
	};
	struct S_CMPNT_MODEL
	{
		std::string					strName = "";
	};
	// model的组件，指向mesh集
	struct S_CMPNT_RELATION_MESH
	{
		std::set<entt::entity>	children;
	};
	// mesh的组件，指向model
	struct S_CMPNT_RELATION_MODEL
	{
		entt::entity			parent;
	};
	struct S_CMPNT_RELATION_TRANSFORM
	{
		entt::entity			parent = entt::null;
		std::set<entt::entity>	children;
		entt::entity			transform_id = entt::null;
		bool					selected_with_parent = false;	// 伴随父节点被选中
		entt::entity			selected_id = entt::null;
	};
}