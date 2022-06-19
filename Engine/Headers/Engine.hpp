#pragma once
#include <chrono>
#include <memory>

#include "ObjectManager/ObjectManager.hpp"
#ifdef CLIENT
#include "Renderer/Renderer.hpp"
#endif

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

	//Getters
	std::shared_ptr<Object> GetObjectByIndex(int index);

	//Others
	float GetDeltaTime();

	int m_Map[81] = {
		1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,1,
		1,0,1,0,1,0,1,0,1,
		1,0,0,0,0,0,0,0,1,
		1,0,1,0,1,0,1,0,1,
		1,0,0,0,0,0,0,0,1,
		1,0,1,0,1,0,1,0,1,
		1,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1
	};

#ifdef CLIENT
	bool IsKeyPressed(sf::Keyboard::Key);
#endif

private:
	//Deltatime
	std::chrono::system_clock::time_point m_StartTime;
	std::chrono::duration<float> m_DeltaTime;
	float m_ElapsedTime;
	unsigned int m_Frames;

	//Components
	std::unique_ptr<ObjectManager> m_ObjectManager;
	std::shared_ptr<Network> m_Network;

#ifdef CLIENT
	std::unique_ptr<Renderer> m_Renderer;
#endif
};