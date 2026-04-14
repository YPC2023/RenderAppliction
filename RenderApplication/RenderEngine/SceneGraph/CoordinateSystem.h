#pragma once
#include <memory>
#include <entt/entt.hpp>
#include "CoordinateAxes.h"
#include "CoordinateArc.h"

class CoordinateSystem
{
public:
	CoordinateSystem();
	~CoordinateSystem();
public:
	entt::entity GetModelId() { return m_ModelId; }
public:
	// 绑定需要通过坐标轴变换的对象ID
	void BindModel(entt::entity entity);
	void UnBindModel();
private:
	entt::entity SetupCoondinateSystem();
private:
	entt::entity CreateCoordinateAxesX();
	entt::entity CreateCoordinateAxesY();
	entt::entity CreateCoordinateAxesZ();

	entt::entity CreateCoordinateArcXY0();
	entt::entity CreateCoordinateArcXY1();
	entt::entity CreateCoordinateArcXY2();
	entt::entity CreateCoordinateArcXY3();

	entt::entity CreateCoordinateArcYZ0();
	entt::entity CreateCoordinateArcYZ1();
	entt::entity CreateCoordinateArcYZ2();
	entt::entity CreateCoordinateArcYZ3();

	entt::entity CreateCoordinateArcXZ0();
	entt::entity CreateCoordinateArcXZ1();
	entt::entity CreateCoordinateArcXZ2();
	entt::entity CreateCoordinateArcXZ3();
private:
	void SetupCoordinateFlag();
private:
	std::shared_ptr<CoordinateAxes>		m_CoordAxesX;
	std::shared_ptr<CoordinateAxes>		m_CoordAxesY;
	std::shared_ptr<CoordinateAxes>		m_CoordAxesZ;

	std::shared_ptr<CoordinateArc>		m_CoordArcXY0;
	std::shared_ptr<CoordinateArc>		m_CoordArcXY1;
	std::shared_ptr<CoordinateArc>		m_CoordArcXY2;
	std::shared_ptr<CoordinateArc>		m_CoordArcXY3;

	std::shared_ptr<CoordinateArc>		m_CoordArcYZ0;
	std::shared_ptr<CoordinateArc>		m_CoordArcYZ1;
	std::shared_ptr<CoordinateArc>		m_CoordArcYZ2;
	std::shared_ptr<CoordinateArc>		m_CoordArcYZ3;

	std::shared_ptr<CoordinateArc>		m_CoordArcXZ0;
	std::shared_ptr<CoordinateArc>		m_CoordArcXZ1;
	std::shared_ptr<CoordinateArc>		m_CoordArcXZ2;
	std::shared_ptr<CoordinateArc>		m_CoordArcXZ3;
private:
	entt::entity	m_BindId;
public:
	entt::entity	m_ModelId;
};