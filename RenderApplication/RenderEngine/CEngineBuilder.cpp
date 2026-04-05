#include "CEngineBuilder.h"
#include "./Session/CSessionManager.h"
#include "CEngine.h"
#include <map>
#include <memory>

static std::map<ISessionInterface*, std::shared_ptr<CSession>>	g_map_session;

IEngineInterface* CEngineBuilder::AquireEngine()
{
	return (IEngineInterface*)&CEngine::GetInstance();
}

ISessionInterface* CEngineBuilder::CreateEngineSession()
{
	std::shared_ptr<CSession> session = CSessionManager::CreateSession();

	ISessionInterface* p = session.get();

	g_map_session[p] = session;

	return p;
}

void CEngineBuilder::ReleaseEngineSession(ISessionInterface* session)
{
	g_map_session.erase(session);
}