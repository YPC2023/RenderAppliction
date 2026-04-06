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
		std::set<entt::entity>		m_set_SelectedId;
		bool						m_RenderID = false;
	};
public:
	static void Initialize(CRenderContext& context, CSceneGraphManager& scene);
	static void Update(CSceneGraphManager& scene);
	static void Render(const CRenderContext& context, const CSceneGraphManager& scene);

private:
	static std::vector<entt::entity> GetTopNode(const CSceneGraphManager& scene);

	static void UpdateNodeRoot(CSceneGraphManager& scene, entt::entity root, entt::entity entity);
	static void UpdateNodeRoot(CSceneGraphManager& scene);

	static void UpdateTransform(CSceneGraphManager& scene);

	static void SortTopologyNode(const CSceneGraphManager& scene,
		entt::entity entity, std::vector<entt::entity>& vecNodes);
	static std::vector<entt::entity> GetTopologyOrder(const CSceneGraphManager& scene);

	static void RenderMesh(const CRenderContext& context, 
		const CSceneGraphManager& scene, entt::entity entity);

	static void ActiveTexture(const CRenderContext& context, 
		std::shared_ptr<CMaterial> material, 
		const CSceneGraphComponent::S_MESH_INFO& mesh);

	static void SetRenderId(const CRenderContext& context, 
		std::shared_ptr<CMaterial> material, 
		entt::entity entity);
private:
	static void SetNeedInitialFlag(bool bYes = true);
	static bool IsNeedInitialize();
private:
	static bool	m_bNeed_Initialize;
};