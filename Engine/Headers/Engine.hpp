#pragma once
#include <chrono>
#include <memory>

#ifdef CLIENT
#include "Renderer/Renderer.hpp"
#endif
#include "ObjectManager/ObjectManager.hpp"

class Object;
class Network;

class Engine
{
public:
	Engine();
	~Engine();

	void Initialize();
	void Begin();
	void End();

	//Compontents
	void SetNetworkComponent(std::shared_ptr<Network> network);

	//Component functions
	bool IsWindowOpen();
	bool HasFocus();
	void SetObjects(std::vector<std::shared_ptr<Object>>& objects);
	//void InitializeObjects();


	//template <typename BaseObject>
	//void AddObject(std::shared_ptr<BaseObject> object)
	//{
	//	m_ObjectManager->AddObject(object);
	//}

	//Getters
	std::shared_ptr<Object> GetObjectByIndex(int index);

	//Others
#ifdef CLIENT
	bool IsKeyPressed(sf::Keyboard::Key);
#endif
	float GetDeltaTime();

private:
	//Deltatime
	std::chrono::system_clock::time_point m_StartTime;
	std::chrono::duration<float> m_DeltaTime;
	float m_ElapsedTime;
	unsigned int m_Frames;

	//Components
	std::unique_ptr<ObjectManager> m_ObjectManager;
#ifdef CLIENT
	std::unique_ptr<Renderer> m_Renderer;
#endif
	std::shared_ptr<Network> m_Network;

};