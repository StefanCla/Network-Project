#include "Precomp.hpp"
#include "ObjectManager/ObjectManager.hpp"

#include "ObjectManager/Object.hpp"
#include "Objects/BombObject.hpp"
#include "Objects/CrateObject.hpp"
#include "Objects/PlayerObject.hpp"
#include "Objects/UpgradeObject.hpp"

ObjectManager::ObjectManager()
{}

ObjectManager::~ObjectManager()
{}

//Update each object if active
void ObjectManager::Update(float deltaTime)
{
	for (auto& object : m_Objects)
	{
		if (object->GetActive())
		{
			object->Update(deltaTime);
		}
	}
}

//Getters
const std::vector<std::shared_ptr<Object>>& ObjectManager::GetObjects() const
{
	return m_Objects;
}

const std::shared_ptr<Object>& ObjectManager::GetObjectByIndex(int index) const
{
	return m_Objects[index];
}

//Setters
void ObjectManager::SetObjects(std::vector<std::shared_ptr<Object>>& objects)
{
	m_Objects = objects;
}