#include "Precomp.hpp"
#include "Objects/TestObject.hpp"

TestObject::TestObject()
{}

TestObject::~TestObject()
{}

void TestObject::Update(float)
{
	//printf("Pos X: %f | Pos Y: %f | Deltatime: %f\n", m_Transform.position.x, m_Transform.position.y, deltaTime);
	printf("Test\n");
}