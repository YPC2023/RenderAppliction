#pragma once
#include "IEngineInterface.h"

class ENGINE_API_ACTION CEngineBuilder
{
public:
	static IEngineInterface* AquireEngine();
	static ISessionInterface* CreateEngineSession();
	static void ReleaseEngineSession(ISessionInterface* session);
};