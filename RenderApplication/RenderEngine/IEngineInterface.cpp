#include "IEngineInterface.h"
#include "CEngine.h"
#include <CUtils.h>
#include "./Session/CSession.h"

static std::map<ISessionInterface*, CSession*>	g_map_session;

IEngineInterface* AquireEngine()
{
	return (IEngineInterface*)&CEngine::GetInstance();
}

void ReleaseEngine()
{
	CEngine::ReleaseIntance();
}

ISessionInterface* CreateSession()
{
	CSession* p = new CSession();
	if (nullptr == p) {
		PRINTLOG("Fail to create session");
		return nullptr;
	}
	return p;
}

void ReleaseSession(ISessionInterface* pInterface)
{
	if (nullptr != pInterface) {
		delete pInterface;
		pInterface = nullptr;
	}
}