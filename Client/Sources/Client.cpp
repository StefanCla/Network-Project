#include "Precomp.h"
#include "Client.h"

#include <iostream>

Client::Client()
{}

Client::~Client()
{}

void Client::PrintNumber(int number)
{
	std::cout << number << std::endl;
}