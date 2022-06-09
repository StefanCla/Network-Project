#pragma once
#include "ObjectManager/Object.hpp"

class PlayerObject : public Object
{
public:
	PlayerObject(unsigned int number);
	~PlayerObject();

	void Update(float deltaTime) override;
private:
	unsigned int number;
};