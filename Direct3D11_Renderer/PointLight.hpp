#pragma once

#include "Entity.hpp"
#include "Drawable.hpp"

class PointLight : public Entity<PointLight>, public Drawable
{
};