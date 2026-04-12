#pragma once
#include <set>
#include <string>
#include <entt/entt.hpp>
#include <ISingletonInterface.h>
#include "../Model/CModelLoader.h"
#include "SceneComponents.h"

class SceneGraph : public ISingletonInterface<SceneGraph>
{
public:
	entt::entity CreateNode();
	void RemoveNode(entt::entity entity);
	template<typename Type, typename... Args>
	decltype(auto) AppendComponent(entt::entity entity, Args &&...args)
	{
		return m_Registry.emplace_or_replace<Type>(entity, std::forward<Args>(args)...);
	}
	template<typename Type, typename... Args>
	decltype(auto) AppendComponent(entt::entity entity, Args &&...args) const
	{
		return m_Registry.emplace_or_replace<Type>(entity, std::forward<Args>(args)...);
	}
	template <typename... Type>
	bool HaveComponent(entt::entity entity) const
	{
		return m_Registry.all_of<Type...>(entity);
	}
	template <typename Type>
	decltype(auto) QueryComponent(entt::entity entity) const
	{
		return m_Registry.get<Type>(entity);
	}
	template <typename Type>
	decltype(auto) QueryComponent(entt::entity entity)
	{
		return m_Registry.get<Type>(entity);
	}
	template <typename... Type>
	decltype(auto) QueryComponentes()  const
	{
		return m_Registry.view<Type...>();
	}
	template <typename... Type>
	decltype(auto) QueryComponentes()
	{
		return m_Registry.view<Type...>();
	}
	bool EntityIsValid(entt::entity entity)
	{
		return m_Registry.valid(entity);
	}
	void Clear()
	{
		m_Registry.clear();
	}
public:
	const SGCmpnt::S_CMPNT_MATERIAL& GetCmpntMaterial(entt::entity entity) const;
	SGCmpnt::S_CMPNT_MATERIAL& GetCmpntMaterial(entt::entity entity);
	const SGCmpnt::S_CMPNT_MESH& GetCmpntMesh(entt::entity entity) const;
	SGCmpnt::S_CMPNT_MESH& GetCmpntMesh(entt::entity entity);
	const SGCmpnt::S_CMPNT_TRANSFORM_DATA& GetCmpntTransformData(entt::entity entity) const;
	SGCmpnt::S_CMPNT_TRANSFORM_DATA& GetCmpntTransformData(entt::entity entity);
	const SGCmpnt::S_CMPNT_MODEL& GetCmpntModel(entt::entity entity) const;
	SGCmpnt::S_CMPNT_MODEL& GetCmpntModel(entt::entity entity);
	const SGCmpnt::S_CMPNT_RELATION_MESH& GetCmpntRelationMesh(entt::entity entity) const;
	SGCmpnt::S_CMPNT_RELATION_MESH& GetCmpntRelationMesh(entt::entity entity);
	const SGCmpnt::S_CMPNT_RELATION_MODEL& GetCmpntRelationModel(entt::entity entity) const;
	SGCmpnt::S_CMPNT_RELATION_MODEL& GetCmpntRelationModel(entt::entity entity);
	const SGCmpnt::S_CMPNT_RELATION_TRANSFORM& GetCmpntRelationTransform(entt::entity entity) const;
	SGCmpnt::S_CMPNT_RELATION_TRANSFORM& GetCmpntRelationTransform(entt::entity entity);
public:
	// 获取entity指定Model的所有Model和Mesh子节点
	const std::set<entt::entity> GetModelTransformComponents(entt::entity entity) const;
	// 获取entity指定的Model及子Model对应的Mesh集
	const std::vector<entt::entity> GetModelTransformMeshComponents(entt::entity entity) const;
	// 获取所有Model的根节点集
	const std::vector<entt::entity> GetModelTopComponentes() const;
	// 根据entity指定的Model节点获取他的所有Model节点
	const void GetModelTopologyComponents(entt::entity entity, std::vector<entt::entity>& vecNodes) const;
	// 获取场景图里的所有Model节点以及的他们的所有子Model节点
	const std::vector<entt::entity> GetModelTopologyComponents() const;
	// 根据entity指定的Model节点获取他的子Mesh节点和子Model的mesh集
	const void GetMeshTopologyComponents(entt::entity entity, std::vector<entt::entity>& vecNodes) const;
	// 获取所有的Mesh节点集
	const std::vector<entt::entity> GetMeshTopologyComponents() const;
public:
	void BindModel(entt::entity parent, entt::entity child, bool selected = true);
	void UnBindModel(entt::entity entity);

	void BindModelTransform(entt::entity parent, entt::entity child);
	void UnBindModelTransform(entt::entity entity);

	void BindModelSelected(entt::entity entity);
	void UnBindModelSelected(entt::entity entity);

public:
	std::string GenPointString(entt::entity parent, entt::entity child) const;
	std::string GetComponentDetail(entt::entity entity) const;
	void GetRelationDetail(std::string& strDetail, entt::entity entity) const;
	std::string GetRelationDetail() const;
public:
	entt::entity CreateModel(const CModel& mode);
	entt::entity CreateMesh(const entt::entity model, const CMesh& mesh);
private:
	entt::registry	m_Registry;
};