#ifndef PROCEDURALWORLD_WORLD_HPP
#define PROCEDURALWORLD_WORLD_HPP

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <vector>

#include <src/HitBox2d.hpp>

#include "Light.h"
#include "Entity.hpp"
#include "Player.hpp"
#include "WorldOrigin.hpp"
#include "WorldTile.hpp"
#include "Text.hpp"

class World: public Entity {
private:
	static int locationToTileIndex(const int& x, const int& z);
	static void tileIndexToLocation(
		const int& index,
		const int& x_center,
		const int& z_center,
		int* const& x,
		int* const& z
	);
	Player player;
	WorldOrigin axes;
    Text menu;
	std::vector<WorldTile*> tiles;
	// current center tile position
	int x_center;
	int z_center;
	GLuint shader_program;
	float player_min_world_y;
	float player_max_world_y;
	bool collidesWith(const HitBox2d& box);
	void placeWorldTile(const int& x, const int& z, const HitBox2d& player_hitbox);
	void checkPosition();
public:
	explicit World(const GLuint& shader_program) : World(shader_program, nullptr) {}
	World(const GLuint& shader_program, Entity* parent)
		: World(shader_program, 0.0f, 0.0f, parent) {}
	World(
		const GLuint& shader_program,
		const float& player_x_start,
		const float& player_z_start
	) : World(shader_program, player_x_start, player_z_start, nullptr) {}
	World(
		const GLuint& shader_program,
		const float& player_x_start,
		const float& player_z_start,
		Entity* parent
	);
	~World() override;


	///////////
	bool pollWorld(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	/////////////

	const Player* getPlayer();
    Text* getMenu();
	void toggleAxes();
	void setPlayerOpacity(const float& opacity);
	void movePlayerForward(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void movePlayerBack(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void movePlayerLeft(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void movePlayerRight(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void movePlayerForwardLeft(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void movePlayerForwardRight(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void movePlayerBackLeft(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void movePlayerBackRight(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
};


#endif //PROCEDURALWORLD_WORLD_HPP
