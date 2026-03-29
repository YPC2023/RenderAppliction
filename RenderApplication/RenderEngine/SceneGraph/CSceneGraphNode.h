#pragma once
#include <set>
#include <entt/entt.hpp>

class CSceneGraphNode
{
public:
	entt::entity			m_parent;
	std::set<entt::entity>	m_children;
};