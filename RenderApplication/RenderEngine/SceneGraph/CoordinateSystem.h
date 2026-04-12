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
	void BindCallback();
private:
	std::shared_ptr<CoordinateAxes>		m_CoordAxesX;
	std::shared_ptr<CoordinateAxes>		m_CoordAxesY;
	std::shared_ptr<CoordinateAxes>		m_CoordAxesZ;
private:
	static void Callback(const glm::mat4& old_value, const glm::mat4& new_value, void* payload);
private:
	entt::entity	m_BindId;
public:
	entt::entity	m_ModelId;
};