#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Renderer/Mesh/BaseMesh.hpp"

class CircleMesh : public BaseMesh
{
public:
	CircleMesh();
	~CircleMesh();

	float m_Diameter;
	glm::vec2 m_Position;
	glm::vec4 m_Color;

};