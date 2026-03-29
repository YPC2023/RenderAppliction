#pragma once
#include <ISingletonInterface.h>
#include "CSceneGraphNode.h"
#include "CSceneGraphComponent.h"
#include <entt/entt.hpp>
#include "../Core/Define.h"

class CSceneGraphManager : public ISingletonInterface<CSceneGraphManager>
{
public:
	entt::entity CreateNode();
	template<typename Type, typename... Args>
	decltype(auto) AppendAttribute(entt::entity entity, Args &&...args)
	{
		return m_Registry.emplace_or_replace<Type>(entity, std::forward<Args>(args)...);
	}
	template <typename... Type>
	bool HaveAttribute(entt::entity entity) const 
	{
		return m_Registry.all_of<Type...>(entity);
	}
	template <typename Type>
	decltype(auto) QueryAttribute(entt::entity entity) const
	{
		return m_Registry.get<Type>(entity);
	}
	template <typename Type>
	decltype(auto) QueryAttributeModify(entt::entity entity)
	{
		return m_Registry.get<Type>(entity);
	}
	template <typename... Type>
	decltype(auto) QueryAttributes()  const
	{
		return m_Registry.view<Type...>();
	}

private:
	entt::registry	m_Registry;
};