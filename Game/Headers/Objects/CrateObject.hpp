#pragma once
#include "ObjectManager/Object.hpp"

class CrateObject : public Object
{
public:
	CrateObject();
	~CrateObject();

	void Update(float deltaTime) override;
private:

};