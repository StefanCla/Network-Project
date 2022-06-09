#pragma once
#include <vector>
#include <memory>

//Note:
//Due to syncing with the server & clients
//I was unable to think of a way to make use of a base object
//Because of this, we have vectors & functions dedicated to each type
//If possible however, i would like to change this

class Object;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	//void InitializeObjects(unsigned int players, unsigned int crates, unsigned int bombs, unsigned int upgrades);

	void Update(float deltaTime);

	//Add to vector
	//template <typename BaseObject>
	//void AddObject(std::shared_ptr<BaseObject> object)
	//{
	//	m_Objects.emplace_back(object);
	//}

	//Getters
	const std::vector<std::shared_ptr<Object>>& GetObjects() const;
	const std::shared_ptr<Object>& GetObjectByIndex(int index) const;

	//Setters
	//Suppose to only set the vector once during loading
	void SetObjects(std::vector<std::shared_ptr<Object>>& objects); 

private:
	std::vector<std::shared_ptr<Object>> m_Objects;

};