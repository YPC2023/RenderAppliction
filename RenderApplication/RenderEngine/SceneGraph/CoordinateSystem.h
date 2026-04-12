#pragma once
#include <memory>
#include <entt/entt.hpp>
#include "CoordinateAxes.h"

class CoordinateSystem
{
public:
	CoordinateSystem();
	~CoordinateSystem();

private:
	void SetupCoondinateSystem();
private:
	std::shared_ptr<CoordinateAxes>		m_CoordAxesX;
	std::shared_ptr<CoordinateAxes>		m_CoordAxesY;
	std::shared_ptr<CoordinateAxes>		m_CoordAxesZ;
};