#include "pch.h"

GameObject::GameObject()
	:pTransform(nullptr)
{
	pTransform = new Transform();
}


GameObject::~GameObject()
{

}
