#include "PhysicsProp.h"
#include "glm/common.hpp"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Renderer.h"

std::vector<b2Vec2> PhysicsProp::GetVerticesFacingPosition(b2Vec2 direction)
{
	return _physics->GetVerticesFacingPosition(direction);
}
