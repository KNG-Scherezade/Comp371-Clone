#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <limits>
#include <algorithm>

#include <src/HitBox2d.hpp>

#include "World.hpp"
#include "../constants.hpp"

World::World(
	const GLuint& shader_program,
	const float& player_x_start,
	const float& player_z_start,
	Entity* parent
) : Entity(parent),
    shader_program(shader_program),
    player(shader_program, this),
    menu(shader_program, "modulus woods\n\n", 0, 0, FONT_STYLE_MYTHOS, this),
    axes(shader_program, WORLD_X_MAX, WORLD_X_MAX, WORLD_Z_MAX, this),

	x_center((int)floor(player_x_start)),
	z_center((int)floor(player_z_start)),
    player_min_world_y(FLT_MAX),
    player_max_world_y(-FLT_MAX)
{
    // hide the axes by default
	this->axes.hide();

	this->player.scale(0.0005f);
	this->player.setPosition(glm::vec3(x_center, 0.01f, z_center));

	// assume player's floating position from ground will remain constant and use
	// the y range of the model to create specialized entity hit boxes for collision
	// detection
    glm::mat4 player_model_matrix = player.getModelMatrix();
	for (const glm::vec3& vertex : this->player.getVertices()) {
        float y = (player_model_matrix * glm::vec4(vertex, 1.0f)).y;
        this->player_min_world_y = std::min(this->player_min_world_y, y);
        this->player_max_world_y = std::max(this->player_max_world_y, y);
	}
	HitBox2d player_starting_hitbox(this->player);
	// populate tiles
	int x, z;
	for (int i = 0; i < 9; i++) {
		World::tileIndexToLocation(i, this->x_center, this->z_center, &x, &z);
		// create tile and add to list of tiles AS WELL AS list of children
		this->tiles.push_back(new WorldTile(
				shader_program,
				x,
				z,
				this->player_min_world_y,
				this->player_max_world_y,
				player_starting_hitbox,
				this
		));
	}

}

World::~World()
{
	for (WorldTile* const& tile : this->tiles) {
		delete tile;
	}
}

const Player* World::getPlayer()
{
	return &this->player;
}

Text* World::getMenu() {
	return &this->menu;
}

bool World::pollWorld(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units) {
    if(player.deadTime != player.deadMax){
        if(player.fore){
            player.moveBack(view_vec, up_vec, units);
        }
        else if(player.back){
            player.moveForward(view_vec, up_vec, units);
        }
        else if(player.left){
            player.moveRight(view_vec, up_vec, units);
        }
        else{
            player.moveLeft(view_vec, up_vec, units);
        }
//        if(player.fore){
//            player.moveForward(view_vec, up_vec, -units / (float)player.deadMax);
//        }
//        else if(player.back){
//            player.moveBack(view_vec, up_vec, -units/ (float)player.deadMax);
//        }
//        else if(player.left){
//            player.moveLeft(view_vec, up_vec, -units/ (float)player.deadMax);
//        }
//        else{
//            player.moveRight(view_vec, up_vec, -units/ (float)player.deadMax);
//        }
        player.deadTime++;
        return true;
    }
    return false;
}

void World::toggleAxes()
{
	this->axes.toggleHide();
}

void World::checkPosition()
{
	static const bool debug_position = false;

	glm::vec3 position = this->player.getPosition();
	if (debug_position) {
		std::cout << "position";
		std::cout << " x " << position.x;
		std::cout << " y " << position.y;
		std::cout << " z " << position.z;
		std::cout << std::endl;
	}

	HitBox2d player_current_hitbox(this->player);

	int diff_x = (int)floor(position.x) - this->x_center;
	int diff_z = (int)floor(position.z) - this->z_center;

	// just to be safe make sure the value is either 1 or -1
	if (diff_x != 0) diff_x = diff_x > 0 ? 1 : -1;
	if (diff_z != 0) diff_z = diff_z > 0 ? 1 : -1;
	// TODO: maybe account for cases where player has moved more than one tile?

	// swap in new column of tiles if needed
	if (diff_x != 0) {
		int new_col_x = this->x_center + diff_x * 2;
		for (int z = this->z_center - 1; z <= this->z_center + 1; z++) {
			this->placeWorldTile(new_col_x, z, player_current_hitbox);
		}
		this->x_center += diff_x;
	}

	// swap in new row of tiles if needed
	if (diff_z != 0) {
		int new_row_z = this->z_center + diff_z * 2;
		for (int x = this->x_center - 1; x <= this->x_center + 1; x++) {
			this->placeWorldTile(x, new_row_z, player_current_hitbox);
		}
		this->z_center += diff_z;
	}
}

void World::placeWorldTile(const int &x, const int &z, const HitBox2d& player_hitbox)
{
	// find tile array index corresponding to new tile location
	int index = World::locationToTileIndex(x, z);
	// get pointer to existing tile occupying this space
	WorldTile* old_tile = this->tiles[index];
	// remove old tile pointer from list of children
	this->detachChild(old_tile);
	// free old tile
	delete old_tile;
	// create new tile, add it to list of children, and replace old tile in tiles array
	this->tiles[index] = new WorldTile(
			this->shader_program,
			x,
			z,
			this->player_min_world_y,
			this->player_max_world_y,
			player_hitbox,
			this
	);
}

int World::locationToTileIndex(const int& x, const int& z)
{
	// NOTE: Mapping position -> tile index is arbitrary, but importantly, consistent!
	// Just like x % 3 except the same pattern continues for negative numbers
	// (3 -> 0, 2 -> 2, 1 -> 1, 0 -> 0, -1 -> 2, -2 -> 1, -3 -> 0, etc)
	int x_index = x - int(floor(x / 3.0f)) * 3;
	int z_index = z - int(floor(z / 3.0f)) * 3;
	return z_index * 3 + x_index;
}

void World::tileIndexToLocation(
	const int& index,
	const int& x_center,
	const int& z_center,
	int* const& x,
	int* const& z
) {
	// TODO: more efficient solution (although this function only gets run once per World instance)
	for (int _x = x_center - 1; _x <= x_center + 1; _x++) {
		for (int _z = z_center - 1; _z <= z_center + 1; _z++) {
			if (index == World::locationToTileIndex(_x, _z)) {
				*x = _x;
				*z = _z;
				return;
			}
		}
	}
	throw std::runtime_error("Tile index should have matching location.");
}

void World::setPlayerOpacity(const float& opacity)
{
	this->player.setOpacity(opacity);
}

void World::movePlayerForward(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units)
{
    if(player.deadTime == player.deadMax) {
        glm::vec3 old_player_position = this->player.getPosition();
        HitBox2d player_hitbox(this->player);
        this->player.moveForward(view_vec, up_vec, units);
        if (this->collidesWith(player_hitbox)) {
            player.deadTime = 0;
            player.fore = true;
            this->player.setPosition(old_player_position);
//            this->player.setPosition(player.oldPosition);
        } else {
            this->checkPosition();
        }
        player.oldPosition = old_player_position;
    }
    else{  }
}

void World::movePlayerBack(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units)
{

    if(player.deadTime == player.deadMax){
        glm::vec3 old_player_position = this->player.getPosition();
        HitBox2d player_hitbox(this->player);
        this->player.moveBack(view_vec, up_vec, units);
        if (this->collidesWith(player_hitbox) ) {
            player.deadTime = 0;
            player.back = true;
            this->player.setPosition(old_player_position);
//            this->player.setPosition(player.oldPosition);
        } else {
            this->checkPosition();
        }
        player.oldPosition = old_player_position;
    }

}

void World::movePlayerLeft(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units)
{
    if(player.deadTime == player.deadMax){
        glm::vec3 old_player_position = this->player.getPosition();
        HitBox2d player_hitbox(this->player);
        this->player.moveLeft(view_vec, up_vec, units);
        if (this->collidesWith(player_hitbox)) {
           player.deadTime = 0;
            player.right = true;
            this->player.setPosition(old_player_position);
//            this->player.setPosition(player.oldPosition);
        } else {
            this->checkPosition();
        }
        player.oldPosition = old_player_position;
    }
}

void World::movePlayerRight(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units)
{
    if(player.deadTime == player.deadMax) {
        glm::vec3 old_player_position = this->player.getPosition();
        HitBox2d player_hitbox(this->player);
        this->player.moveRight(view_vec, up_vec, units);
        if (this->collidesWith(player_hitbox)) {
//            this->player.setPosition(player.oldPosition);
            this->player.setPosition(old_player_position);
            player.deadTime = 0;
            player.left = true;
        } else {
            this->checkPosition();
        }
        //player.oldPosition = old_player_position;
    }
}

void World::movePlayerForwardLeft(
	const glm::vec3& view_vec,
	const glm::vec3& up_vec,
	const float& units
) {

	glm::vec3 old_player_position = this->player.getPosition();
	HitBox2d player_hitbox(this->player);
	this->player.moveForwardLeft(view_vec, up_vec, units);
	if (this->collidesWith(player_hitbox)) {
        this->player.setPosition(player.oldPosition);
    } else {
        this->checkPosition();
    }
    player.oldPosition = old_player_position;

}

void World::movePlayerForwardRight(
	const glm::vec3& view_vec,
	const glm::vec3& up_vec,
	const float& units
) {

	glm::vec3 old_player_position = this->player.getPosition();
	HitBox2d player_hitbox(this->player);
	this->player.moveForwardRight(view_vec, up_vec, units);
	if (this->collidesWith(player_hitbox)) {
        this->player.setPosition(player.oldPosition);
    } else {
        this->checkPosition();
    }
    player.oldPosition = old_player_position;
}

void World::movePlayerBackLeft(
	const glm::vec3& view_vec,
	const glm::vec3& up_vec,
	const float& units
) {
	glm::vec3 old_player_position = this->player.getPosition();
	HitBox2d player_hitbox(this->player);
	this->player.moveBackLeft(view_vec, up_vec, units);
	if (this->collidesWith(player_hitbox)) {
        this->player.setPosition(player.oldPosition);
    } else {
        this->checkPosition();
    }
    player.oldPosition = old_player_position;
}

void World::movePlayerBackRight(
	const glm::vec3& view_vec,
	const glm::vec3& up_vec,
	const float& units
) {
	glm::vec3 old_player_position = this->player.getPosition();
	HitBox2d player_hitbox(this->player);
	this->player.moveBackRight(view_vec, up_vec, units);
	if (this->collidesWith(player_hitbox)) {
        this->player.setPosition(player.oldPosition);
    } else {
        this->checkPosition();
    }
    player.oldPosition = old_player_position;
}

bool World::collidesWith(const HitBox2d& box)
{
	for (const WorldTile* tile : this->tiles) {
		if (tile->collidesWith(box)) {
			return true;
		}
	}
	return false;
}
