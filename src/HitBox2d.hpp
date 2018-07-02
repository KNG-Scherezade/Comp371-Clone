//
// Created by Ben Wiley on 2017-11-26.
//

#ifndef PROCEDURALWORLD_HITBOX2D_HPP
#define PROCEDURALWORLD_HITBOX2D_HPP

#include <limits>
#include <iostream>

#include "entities/DrawableEntity.hpp"

// Class that can test if one x-z box collides with another - can be specified directly
// or generated via DrawableEntity and y range

class HitBox2d {
	friend std::ostream& operator<<(std::ostream& os, const HitBox2d& box);
private:
	float world_min_x;
	float world_min_z;
	float world_max_x;
	float world_max_z;
public:
	explicit HitBox2d(const DrawableEntity& entity, const float& min_y = -FLT_MAX, const float& max_y = FLT_MAX);
	HitBox2d(const float& min_x, const float& min_z, const float& max_x, const float& max_z);
	bool collidesWith(const HitBox2d& box) const;
};


#endif //PROCEDURALWORLD_HITBOX2D_HPP
