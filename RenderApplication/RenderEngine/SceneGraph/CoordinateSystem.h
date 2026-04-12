#pragma once
#include <memory>
#include <entt/entt.hpp>
#include "CoordinateAxes.h"

class CoordinateSystem
{
public:
	CoordinateSystem();
	~CoordinateSystem();
public:
	entt::entity GetModelId() { return m_ModelId; }
private:
	entt::entity SetupCoondinateSystem();
private:
	std::shared_ptr<CoordinateAxes>		m_CoordAxesX;
	std::shared_ptr<CoordinateAxes>		m_CoordAxesY;
	std::shared_ptr<CoordinateAxes>		m_CoordAxesZ;

public:
	entt::entity	m_ModelId;
};