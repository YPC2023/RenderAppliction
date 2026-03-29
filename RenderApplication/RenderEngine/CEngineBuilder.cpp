#include "CEngineBuilder.h"
#include "./Session/CSessionManager.h"
#include <map>
#include <memory>

static std::map<IEngineInterface*, std::shared_ptr<CSession>>	g_map_session;

IEngineInterface* CEngineBuilder::CreateEngineSession()
{
	std::shared_ptr<CSession> session = CSessionManager::CreateSession();

	IEngineInterface* p = session.get();

	g_map_session[p] = session;

	return p;
}

void CEngineBuilder::ReleaseEngineSession(IEngineInterface* session)
{
	g_map_session.erase(session);
}