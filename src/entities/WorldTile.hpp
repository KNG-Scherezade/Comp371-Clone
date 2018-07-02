#ifndef PROCEDURALWORLD_WORLDTILE_H
#define PROCEDURALWORLD_WORLDTILE_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <vector>

#include <src/HitBox2d.hpp>

#include "Entity.hpp"
#include "Rock.hpp"
#include "Trees/Tree.hpp"
#include "DrawableEntity.hpp"
#include "Rock.hpp"
#include "RockB.hpp"
#include "Text.hpp"

class WorldTile: public DrawableEntity {
private:
	std::vector<Rock*> rocks;
    std::vector<RockB*> rocksB;
	std::vector<Tree*> trees;
	std::vector<HitBox2d> hitboxes;
    Text seed_loc_message;
public:
	WorldTile(
		const GLuint& shader_program,
		const int& world_x_location,
		const int& world_z_location,
		const float& min_hitbox_y,
		const float& max_hitbox_y,
		const HitBox2d& player_hitbox
    ) : WorldTile(
		shader_program,
		world_x_location,
		world_z_location,
		min_hitbox_y,
		max_hitbox_y,
		player_hitbox,
		nullptr
	) {}
	WorldTile(
		const GLuint& shader_program,
		const int& world_x_location,
		const int& world_z_location,
		const float& min_hitbox_y,
		const float& max_hitbox_y,
		const HitBox2d& player_hitbox,
		Entity* parent
	);
	~WorldTile() override;
	const std::vector<glm::vec3>& getVertices() const override;
	GLuint getVAO() override;
	const int getColorType() override;
	bool collidesWith(const HitBox2d& box) const;
};

#endif //PROCEDURALWORLD_WORLDTILE_H
