#pragma once
#include "../Session/CSessionManager.h"
#include "../SceneGraph/CSceneGraphManager.h"
#include "../SceneGraph/CSceneGraphComponent.h"




class CRenderSystem
{
public:
	struct CRenderContext
	{
		std::shared_ptr<CMaterial>	m_Material = nullptr;
		std::shared_ptr<Camera>		m_Camera = nullptr;
		entt::entity				m_SelectedId = entt::null;
		bool						m_RenderID = false;
	};
public:
	static void Update(CSceneGraphManager& scene);
	static void Render(const CRenderContext& context, const CSceneGraphManager& scene);

private:
	static void UpdateNode(CSceneGraphManager& scene, entt::entity entity);

	static void RenderMesh(const CRenderContext& context, 
		const CSceneGraphManager& scene, entt::entity entity);

	static void ActiveTexture(const CRenderContext& context, 
		std::shared_ptr<CMaterial> material, 
		const CSceneGraphComponent::S_MESH_INFO& mesh);

	static void SetRenderId(const CRenderContext& context, 
		std::shared_ptr<CMaterial> material, 
		entt::entity entity);
};