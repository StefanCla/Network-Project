#pragma once
#include <glm/vec2.hpp>

struct Transform
{
	Transform()
	{}

	Transform(glm::vec2 pos, glm::vec2 rot, glm::vec2 scal)
		: position(pos)
		, rotation(rot)
		, scale(scal)
	{}

	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	glm::vec2 rotation = glm::vec2(0.0f, 0.0f);
	glm::vec2 scale = glm::vec2(0.0f, 0.0f);
};