#include "Precomp.hpp"
#include "ObjectManager/Object.hpp"

Object::Object()
	: m_Active(false)
	, m_RectSize(32.0f, 32.0f)
	, m_MeshType(MeshType::Rect)
	, m_Cell(10)
{
	m_Mesh = std::make_unique<RectMesh>();
}

Object::~Object()
{}

void Object::AddMeshType(MeshType meshType)
{
	switch (meshType)
	{
	case MeshType::Circle:
		m_Mesh = std::make_unique<CircleMesh>();
		m_MeshType = MeshType::Circle;
		break;
	case MeshType::Rect:
		m_Mesh = std::make_unique<RectMesh>();
		m_MeshType = MeshType::Rect;
		break;
	}
}

//Getters
bool Object::GetActive()
{
	return m_Active;
}

Transform& Object::GetTransform()
{
	return m_Transform;
}

const glm::vec2& Object::GetRectSize()
{
	return m_RectSize;
}

const MeshType Object::GetMeshType()
{
	return m_MeshType;
}

BaseMesh* Object::GetMesh()
{
	return m_Mesh.get();
}

int Object::GetCell()
{
	return m_Cell;
}

//Setters
void Object::SetActive(bool active)
{
	m_Active = active;
}

void Object::SetCell(int cell)
{
	m_Cell = cell;
}