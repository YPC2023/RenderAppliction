#pragma once
#include "IEngineInterface.h"

class ENGINE_API_ACTION CEngineBuilder
{
public:
	static IEngineInterface* CreateEngineSession();
	static void ReleaseEngineSession(IEngineInterface* session);
};