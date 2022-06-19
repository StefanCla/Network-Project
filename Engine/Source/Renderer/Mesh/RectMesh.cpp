#include "Precomp.hpp"
#include "Renderer/Mesh/RectMesh.hpp"

RectMesh::RectMesh() :
	m_ShapeSize(glm::vec2(32.0f, 32.0f)),
	m_Position(glm::vec2(0.0f, 0.0f)),
	m_Color(glm::vec4(255, 255, 0, 0))
{}

RectMesh::~RectMesh()
{}