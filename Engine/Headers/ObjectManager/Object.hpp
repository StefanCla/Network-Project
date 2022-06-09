#pragma once
#include <glm/vec2.hpp>
#include "ObjectManager/Transform.hpp"

#include "Renderer/Mesh/BaseMesh.hpp"
#include "Renderer/Mesh/CircleMesh.hpp"
#include "Renderer/Mesh/RectMesh.hpp"

enum class MeshType
{
	Circle,
	Rect,
	Texture
};

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void Update(float deltaTime) = 0;
	void AddMeshType(MeshType meshType);

	//Getters
	bool GetActive();
	Transform& GetTransform();
	const glm::vec2& GetRectSize();
	const MeshType GetMeshType();
	BaseMesh* GetMesh();

	//Setters
	void SetActive(bool active);

protected:
	bool m_Active;
	Transform m_Transform;
	glm::vec2 m_RectSize;
	std::unique_ptr<BaseMesh> m_Mesh;
	MeshType m_MeshType;

};