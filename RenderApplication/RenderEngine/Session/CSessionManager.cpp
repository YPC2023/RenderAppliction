#include "CSessionManager.h"

CSession* CSessionManager::m_CurrentSession = nullptr;

std::shared_ptr<CSession> CSessionManager::CreateSession()
{
	return std::make_shared<CSession>();
}

void CSessionManager::SwitchCurrentSession(CSession* session)
{
	m_CurrentSession = session;
}