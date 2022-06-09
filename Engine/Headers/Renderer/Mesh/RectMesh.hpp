#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Renderer/Mesh/BaseMesh.hpp"

class RectMesh : public BaseMesh
{
public:
	RectMesh();
	~RectMesh();

	glm::vec2 m_ShapeSize;
	glm::vec2 m_Position;
	glm::vec4 m_Color;
};