#include <glm/glm.hpp>
#include <algorithm>
#include <vector>
#include <limits>
#include <iostream>

#include "entities/DrawableEntity.hpp"
#include "HitBox2d.hpp"

HitBox2d::HitBox2d(const DrawableEntity& entity, const float& min_y, const float& max_y)
	: world_min_x(FLT_MAX), world_min_z(FLT_MAX), world_max_x(-FLT_MAX), world_max_z(-FLT_MAX)
{
	glm::vec3 v;
	const glm::mat4& model_matrix = entity.getModelMatrix();
    for (const glm::vec3& vertex : entity.getVertices()) {
        	v = glm::vec3(model_matrix * glm::vec4(vertex, 1.0f));
        	if (v.y < min_y || v.y > max_y) {
            		// we don't want to include vertices outside the specified y range in our box
                    		continue;
            	}
        	this->world_min_x = std::min(v.x, this->world_min_x);
        	this->world_min_z = std::min(v.z, this->world_min_z);
        	this->world_max_x = std::max(v.x, this->world_max_x);
        	this->world_max_z = std::max(v.z, this->world_max_z);
	}
}

HitBox2d::HitBox2d(
	const float& min_x,
	const float& min_z,
	const float& x_span,
	const float& z_span
) : world_min_x(min_x),
    world_min_z(min_z),
    world_max_x(min_x + x_span),
    world_max_z(min_z + z_span) {}

bool HitBox2d::collidesWith(const HitBox2d& box) const
{
	// basic rectangle intersection test
	bool r = this->world_min_x < box.world_max_x &&
			this->world_max_x > box.world_min_x &&
			this->world_min_z < box.world_max_z &&
			this->world_max_z > box.world_min_z;
//	if (r) 	std::cout << "this " << *this << std::endl;
//	if (r) 	std::cout << "box " << box << std::endl;
	return r;
}

std::ostream& operator<<(std::ostream& os, const HitBox2d& box)
{
	os << "BL: (" << box.world_min_x << ", " << box.world_min_z << "), ";
	os << "TR: (" << box.world_max_x << ", " << box.world_max_z << ")";
	return os;
}

