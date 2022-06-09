#pragma once
#include "ObjectManager/Object.hpp"

class UpgradeObject : public Object
{
public:
	UpgradeObject();
	~UpgradeObject();

	void Update(float deltaTime) override;
private:

};