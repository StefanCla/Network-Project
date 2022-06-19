#include "Precomp.hpp"
#include "Objects/PlayerObject.hpp"

PlayerObject::PlayerObject(unsigned int number)
	: number(number)
{}

PlayerObject::~PlayerObject()
{}

void PlayerObject::Update(float)
{
	printf("Cell: % i\n", m_Cell);
}