#include "Precomp.hpp"
#include "Renderer/Mesh/CircleMesh.hpp"

CircleMesh::CircleMesh() :
	m_Diameter(5.0f),
	m_Position(glm::vec2(0.0f, 0.0f)),
	m_Color(glm::vec4(255, 255, 0, 0))
{}

CircleMesh::~CircleMesh()
{}