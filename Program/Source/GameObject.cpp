#include "GameObject.h"
#include <iostream>

void GameObject::ShowTooltip()
{
	//std::cout << "ShowTooltip from base" << std::endl;
}

glm::vec2 GameObject::GetPosition()
{
	return glm::vec2();
}

bool GameObject::IsPointOverlapping(glm::vec2 point)
{
	return true;
}

void GameObject::ShowUI()
{
}

void GameObject::Draw()
{
}

void GameObject::Update()
{
}
