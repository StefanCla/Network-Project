#pragma once
#include "ObjectManager/Object.hpp"

class BombObject : public Object
{
public:
	BombObject();
	~BombObject();

	void Update(float deltaTime) override;
private:

};