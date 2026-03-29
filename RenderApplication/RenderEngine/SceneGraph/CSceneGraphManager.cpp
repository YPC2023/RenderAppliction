#include "CSceneGraphManager.h"
#include <CUtils.h>

entt::entity CSceneGraphManager::CreateNode()
{
	const auto entity = m_Registry.create();
	PRINTLOG("id=%u", entity);
	(void)AppendAttribute<CSceneGraphNode>(entity);
	return entity;
}
