#pragma once
#include "../Session/CSessionManager.h"
#include "../SceneGraph/SceneGraph.h"

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
	static void Initialize(CRenderContext& context, SceneGraph& scene);
	static void Update(SceneGraph& scene);
	static void Render(const CRenderContext& context, const SceneGraph& scene);

private:
	static void UpdateModel(SceneGraph& scene);
	static void UpdateMesh(SceneGraph& scene);

	static void RenderMesh(const CRenderContext& context, 
		const SceneGraph& scene, entt::entity entity);

	static void ActiveTexture(const CRenderContext& context, 
		std::shared_ptr<CMaterial> material, 
		const SGCmpnt::S_CMPNT_MESH& mesh);

	static void SetRenderId(const CRenderContext& context, 
		std::shared_ptr<CMaterial> material, 
		entt::entity entity);
private:
	static void SetNeedInitialFlag(bool bYes = true);
	static bool IsNeedInitialize();
private:
	static bool	m_bNeed_Initialize;
};