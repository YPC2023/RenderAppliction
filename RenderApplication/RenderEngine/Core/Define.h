#pragma once

#include <glm/glm.hpp>

using entity_type = std::uint32_t;

#define IDM_VIEWPORT_WIDTH   800
#define IDM_VIEWPORT_HEIGHT  600
static const glm::vec3 CONST_CAMERA_POS = glm::vec3(5.0f, 5.0f, 5.0f);
static const glm::vec3 CONST_CAMERA_TARGET = glm::vec3(0.0f, 0.0f, 0.0f);
static const glm::vec3 CONST_CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
static const float CONST_CAMERA_FOV = 90.0f;
static const float CONST_CAMERA_NEAR = 0.1f;
static const float CONST_CAMERA_FAR = 100.0f;