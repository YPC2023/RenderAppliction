#pragma once

#include <set>
#include <string>
#include <memory>
#include "../SceneGraph/Camera.h"
#include "../SceneGraph/SceneGraph.h"

class CRenderSystem
{
public:
	struct CRenderContext
	{
		std::shared_ptr<CMaterial>	m_Material = nullptr;
		std::shared_ptr<Camera>		m_Camera = nullptr;
		std::set<entt::entity>		m_set_SelectedId;
		std::set<entt::entity>		m_set_Unvisible;	// 当前渲染（session）看不到的对象集
		bool						m_RenderID = false;
	};
public:
	static void Update(const CRenderContext& context, SceneGraph& scene);
	static void Render(const CRenderContext& context, const SceneGraph& scene);

private:
	static void UpdateModel(const CRenderContext& context, SceneGraph& scene);
	static void UpdateMesh(const CRenderContext& context, SceneGraph& scene);

	static void RenderMesh(const CRenderContext& context, 
		const SceneGraph& scene, entt::entity entity);

	static void ActiveTexture(const CRenderContext& context, 
		std::shared_ptr<CMaterial> material, 
		const SGCmpnt::S_CMPNT_MESH& mesh);

	static void SetRenderId(const CRenderContext& context, 
		std::shared_ptr<CMaterial> material, 
		entt::entity entity);

private:
	static std::string FormatVec3(const glm::vec3& value);
	static std::string FormatVec4(const glm::vec4& value);
	static std::string FormatMat4(const glm::mat4& value);
	static void PrintMat4(const glm::mat4& value);
};