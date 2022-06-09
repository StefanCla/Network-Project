#pragma once
#include "ObjectManager/Object.hpp"

class TestObject : public Object
{
public:
	TestObject();
	~TestObject();

	//void Update(float deltaTime) override;

private:
	void Update(float deltaTime) override;

};
