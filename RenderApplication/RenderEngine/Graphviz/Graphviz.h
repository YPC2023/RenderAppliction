#pragma once
#include <map>
#include <string>
#include <ISingletonInterface.h>
#include "../SceneGraph/SceneGraph.h"
#include <graphviz/gvc.h>

class Graphviz : public ISingletonInterface<Graphviz>
{
private:
	typedef struct _S_CONTEXT
	{
		const SceneGraph& scene;
		Agraph_t* g = nullptr;
		GVC_t* gvc = nullptr;
		_S_CONTEXT(const SceneGraph& s):scene(s)
		{
		}
	}S_CONTEXT;
public:
	void RenderGraph(const SceneGraph& scene, const std::string& strPath);
private:
	void RenderToDot(const S_CONTEXT& cxt, const std::string& strPath);
	void RenderToSvg(const S_CONTEXT& cxt, const std::string& strPath);
private:
	std::string RenderContent(const std::string& strPath, const inja::json& data);
	inja::json ModelToInjaJson(const S_CONTEXT& cxt, entt::entity entity);
	inja::json MeshToInjaJson(const S_CONTEXT& cxt, entt::entity entity);
private:
	std::string GetModelContent(const S_CONTEXT& cxt, entt::entity entity);
	std::string GetMeshContent(const S_CONTEXT& cxt, entt::entity entity);
private:
	void SetModelNodeStyle(Agnode_t* node);
	void SetMeshNodeStyle(Agnode_t* node);

	void CreateModelNode(const S_CONTEXT& cxt, entt::entity entity);
	void CreateMeshNode(const S_CONTEXT& cxt, entt::entity entity);

	void LinkModelModel(const S_CONTEXT& cxt, entt::entity parent, entt::entity child);
	void LinkModelMesh(const S_CONTEXT& cxt, entt::entity parent, entt::entity child);
private:
	void RenderModel(const S_CONTEXT& cxt);
	void RenderMesh(const S_CONTEXT& cxt);


private:
	std::map<entt::entity, Agnode_t*>	m_map_Node;
};