#pragma once
#include <vector>
#include <memory>
#include "CSession.h"

class CSessionManager
{
public:
	static std::shared_ptr<CSession> CreateSession();
	static void SwitchCurrentSession(CSession* session);
	static CSession* GetCurrentSession() { return m_CurrentSession; }
private:
	static CSession*	m_CurrentSession;
};