#include "Precomp.hpp"
#include "Objects/PlayerObject.hpp"

PlayerObject::PlayerObject(unsigned int number)
	: number(number)
{}

PlayerObject::~PlayerObject()
{}

void PlayerObject::Update(float)
{
	printf("Player: %i\n", number);
}