#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>

#include <src/utils.hpp>
#include <src/constants.hpp>
#include <src/HitBox2d.hpp>

#include "Entity.hpp"
#include "DrawableEntity.hpp"
#include "Rock.hpp"

#include "src/entities/Trees/Tree.hpp"
#include "src/entities/Trees/TreeA.hpp"
#include "src/entities/Trees/TreeA_Autumn.hpp"
#include "src/entities/Trees/TreeB.hpp"
#include "src/entities/Trees/TreeC.hpp"
#include "WorldTile.hpp"
#include "Tentacle.hpp"


WorldTile::WorldTile(
	const GLuint &shader_program,
	const int& world_x_location,
	const int& world_z_location,
	const float& min_hitbox_y,
	const float& max_hitbox_y,
	const HitBox2d& player_hitbox,
	Entity *parent
) : DrawableEntity(shader_program, parent),
    seed_loc_message(
		    shader_program,
		    "Seed for current location: " + std::to_string(world_x_location) + ':' + std::to_string(world_z_location),
		    0,
		    0,
		    FONT_STYLE_OUTLINE,
		    this
    )

{
	this->draw_mode = GL_TRIANGLES;

	this->setMaterial(
		glm::vec3(.5,.5,.5), // need to change this to some other value... maybe the height of the plane if we ever make it.
		glm::vec3(.5,.5,.5),
		glm::vec3(.25,.25,.25),
		25.0f
	);

    seed_loc_message.setPosition(glm::vec3(0.5, -0.96, 0.5));

	// position tile relative to parent based on x, z inputs
	this->translate(glm::vec3(world_x_location, 0.0f, world_z_location));

	// initialize random number generator based on world location
	srand((unsigned int)(world_x_location * world_z_location + world_x_location + world_z_location));

	// TODO: better tree/rock distribution?
	// TODO: test/remove tree/rock overlaps

    const int generalBiomeX = 5, generalBiomeY = 5;
    const int alienBiomeX = 5, alienBiomeY = 10;
    const int tentacleX = 10, tentacleY = 5;
    const int heavyRenderX = 10, heavyRenderY = 10;
    const int worldBoundries = 10;

	// add rocks
	int rock_tries_left = 3;
	for (int i = 0; i < 7; i++) {
		float x_span = utils::randomFloat(0.02f, 0.05f);
		float z_span = utils::randomFloat(0.02f, 0.05f);
        float y_span = utils::randomFloat(0.005f, 0.015f);
		float x_position = utils::randomFloat(0.0f, 1.0f - x_span);
		float z_position = utils::randomFloat(0.0f, 1.0f - z_span);

		HitBox2d box(x_position, z_position, x_position + x_span, z_position + z_span);
		if (this->collidesWith(box) || player_hitbox.collidesWith(box)) {
			if (rock_tries_left-- > 0) {
				// collision! but give it another try
				i--;
			}
			continue;
		}

        //general biome
        if (abs(world_x_location) % worldBoundries < generalBiomeX && abs(world_z_location) % worldBoundries < generalBiomeY) {
            if((int)(ceil(x_position * y_span + world_z_location)) % 2 == 0){
                // Add rock child
                RockB* rockB = new RockB(
                        shader_program,
                        world_x_location + x_position,
                        world_z_location + z_position,
                        x_span,
                        z_span,
                        this
                );
                rockB->setPosition(glm::vec3(x_position, 0.002f, z_position));

                rockB->scale(glm::vec3(x_span, y_span, z_span));
                // Add rock to rocks array
                this->rocksB.emplace_back(rockB);
                this->hitboxes.emplace_back(*rockB, min_hitbox_y, max_hitbox_y);
            }
            else{
                // Add rock child
                Rock* rockA = new Rock(
                        shader_program,
                        world_x_location + x_position,
                        world_z_location + z_position,
                        x_span,
                        z_span,
                        this
                );
                rockA->setPosition(glm::vec3(x_position, 0.002f, z_position));

                rockA->scale(glm::vec3(x_span, y_span, z_span));
                // Add rock to rocks array
                this->rocks.emplace_back(rockA);
                this->hitboxes.emplace_back(*rockA, min_hitbox_y, max_hitbox_y);
            }
        }



            //Alien biome
        else if (abs(world_x_location) % worldBoundries < alienBiomeX  && abs(world_z_location) % worldBoundries < alienBiomeY ) {
            // Add rock child
            RockB* rockB = new RockB(
                    shader_program,
                    world_x_location + x_position,
                    world_z_location + z_position,
                    x_span,
                    z_span,
                    this
            );
            rockB->setPosition(glm::vec3(x_position, 0.002f, z_position));
            rockB->scale(glm::vec3(x_span, y_span, z_span));
            // Add rock to rocks array
            this->rocksB.emplace_back(rockB);
            this->hitboxes.emplace_back(*rockB, min_hitbox_y, max_hitbox_y);


            float x_span = utils::randomFloat(0.02f, 0.05f);
            float z_span = utils::randomFloat(0.02f, 0.05f);
            float y_span = utils::randomFloat(0.005f, 0.015f);
            float x_position = utils::randomFloat(0.0f, 1.0f - x_span);
            float z_position = utils::randomFloat(0.0f, 1.0f - z_span);
            // Add rock child
            Rock* rockA = new Rock(
                    shader_program,
                    world_x_location + x_position,
                    world_z_location + z_position,
                    x_span,
                    z_span,
                    this
            );
            rockA->setPosition(glm::vec3(x_position, 0.002f, z_position));
            rockA->scale(glm::vec3(x_span, y_span, z_span));
            // Add rock to rocks array
            this->rocks.emplace_back(rockA);
            this->hitboxes.emplace_back(*rockA, min_hitbox_y, max_hitbox_y);
        }



            //Tentacle Biome
        else if(abs(world_x_location) % worldBoundries < tentacleX && abs(world_z_location) % worldBoundries < tentacleY) {
                // Add rock child
                RockB* rockB = new RockB(
                        shader_program,
                        world_x_location + x_position,
                        world_z_location + z_position,
                        x_span,
                        z_span,
                        this
                );
                rockB->setPosition(glm::vec3(x_position, 0.002f, z_position));
                rockB->scale(glm::vec3(x_span, y_span, z_span));
                // Add rock to rocks array
                this->rocksB.emplace_back(rockB);
                this->hitboxes.emplace_back(*rockB, min_hitbox_y, max_hitbox_y);

            float x_span = utils::randomFloat(0.02f, 0.05f);
            float z_span = utils::randomFloat(0.02f, 0.05f);
            float y_span = utils::randomFloat(0.005f, 0.015f);
            float x_position = utils::randomFloat(0.0f, 1.0f - x_span);
            float z_position = utils::randomFloat(0.0f, 1.0f - z_span);
                // Add rock child
                Rock* rockA = new Rock(
                        shader_program,
                        world_x_location + x_position,
                        world_z_location + z_position,
                        x_span,
                        z_span,
                        this
                );
                rockA->setPosition(glm::vec3(x_position, 0.002f, z_position));

                rockA->scale(glm::vec3(x_span, y_span, z_span));
                // Add rock to rocks array
                this->rocks.emplace_back(rockA);
                this->hitboxes.emplace_back(*rockA, min_hitbox_y, max_hitbox_y);
        }



            //forest biome (A and C) heavyRenderX heavyRenderY
        else{
            // Add rock child
            RockB* rockB = new RockB(
                    shader_program,
                    world_x_location + x_position,
                    world_z_location + z_position,
                    x_span,
                    z_span,
                    this
            );
            rockB->setPosition(glm::vec3(x_position, 0.002f, z_position));

            rockB->scale(glm::vec3(x_span, y_span, z_span));
            // Add rock to rocks array
            this->rocksB.emplace_back(rockB);
            this->hitboxes.emplace_back(*rockB, min_hitbox_y, max_hitbox_y);

            float x_span = utils::randomFloat(0.02f, 0.05f);
            float z_span = utils::randomFloat(0.02f, 0.05f);
            float y_span = utils::randomFloat(0.005f, 0.015f);
            float x_position = utils::randomFloat(0.0f, 1.0f - x_span);
            float z_position = utils::randomFloat(0.0f, 1.0f - z_span);

            // Add rock child
            Rock* rockA = new Rock(
                    shader_program,
                    world_x_location + x_position,
                    world_z_location + z_position,
                    x_span,
                    z_span,
                    this
            );
            rockA->setPosition(glm::vec3(x_position, 0.002f, z_position));

            rockA->scale(glm::vec3(x_span, y_span, z_span));
            // Add rock to rocks array
            this->rocks.emplace_back(rockA);
            this->hitboxes.emplace_back(*rockA, min_hitbox_y, max_hitbox_y);
            break;
            }
	}


	//enable tree distributor function
	//TreeDistributor::setEntity(this);
	// add trees
	int tree_tries_left = 2;
	for (int i = 0; i < 5; i++) {
		static const float scale_factor = 100;
		float base_span = utils::randomFloat(0.02f, 0.05f);
		float internal_tree_width = base_span * scale_factor;
		float x_position = utils::randomFloat(0.0f, 1.0f - base_span);
		float z_position = utils::randomFloat(0.0f, 1.0f - base_span);

		HitBox2d box(x_position, z_position, x_position + base_span, z_position + base_span);
		if (this->collidesWith(box) || player_hitbox.collidesWith(box)) {
			if (tree_tries_left-- > 0) {
				// collision! but give it another try
				i--;
			}
			continue;
		}

		int seed = std::abs((world_x_location + x_position) * (world_z_location + z_position))*scale_factor;
		seed = seed == 0 ? (world_x_location + x_position + world_z_location + z_position+3 )* 7: seed;
        // Add tree child
        Tree *tree;



        //general biome
        if (abs(world_x_location) % worldBoundries < generalBiomeX && abs(world_z_location) % worldBoundries < generalBiomeY) {
            bool isAlien = false;
            if (seed % worldBoundries < 2) {
                if (seed % 2 == 0)
                    tree = new TreeA(shader_program, this, internal_tree_width * 2.5, seed, isAlien);
                else
                    tree = new TreeA_Autumn(shader_program, this, internal_tree_width * 2.5, seed);
            } else if (seed % worldBoundries < 7) {
                tree = new TreeB(shader_program, this, internal_tree_width, seed, isAlien);
            } else {
                TreeC tc(seed % 15, shader_program, this, internal_tree_width, seed, isAlien, trees,
                         {x_position, 0.0f, z_position}, 1.0f / (scale_factor*10), min_hitbox_y, max_hitbox_y, hitboxes);

                continue;
            }
        }



            //Alien biome
        else if (abs(world_x_location) % worldBoundries < alienBiomeX  && abs(world_z_location) % worldBoundries < alienBiomeY ) {
            bool isAlien = true;
            if (seed % worldBoundries < 2) {
                    tree = new TreeA(shader_program, this, internal_tree_width * 2.5, seed, isAlien);
            } else if (seed % worldBoundries < 7) {
                tree = new TreeB(shader_program, this, internal_tree_width, seed, isAlien);
            } else {
                TreeC tc(seed % 15, shader_program, this, internal_tree_width, seed, isAlien, trees,
                         {x_position, 0.0f, z_position}, 1.0f / (scale_factor*10), min_hitbox_y, max_hitbox_y, hitboxes);

                continue;
            }
        }



            //Tentacle Biome
        else if(abs(world_x_location) % worldBoundries < tentacleX && abs(world_z_location) % worldBoundries < tentacleY) {
            bool isAlien, isTextured;
            if(seed % 3 == 0) {
                isAlien = false, isTextured = true;
            }
           else if(seed % 3 == 1)
               isAlien = true, isTextured = true;
            else
                isAlien = true, isTextured = false;

            tree =  new Tentacle(shader_program, this, internal_tree_width * 2.5, seed, isAlien, isTextured);
        }



            //forest biome (A and C) heavyRenderX heavyRenderY
        else{
            bool isAlien = false;
            if (seed % 10 < 7) {
                if (seed % 2 == 0)
                    tree = new TreeA(shader_program, this, internal_tree_width * 3, seed, isAlien);
                else
                    tree = new TreeA_Autumn(shader_program, this, internal_tree_width * 3, seed);
            } else {

                TreeC::setSpacingConstant(10);
                TreeC tc(0, shader_program, this, internal_tree_width * 1.5, seed, isAlien, trees,
                         {x_position, 0.0f, z_position}, 1.0f / (scale_factor*10), min_hitbox_y, max_hitbox_y, hitboxes);
                TreeC::setSpacingConstant(10);
                continue;
            }
        }

		tree->setPosition(glm::vec3(x_position, 0.0f, z_position));
		tree->scale(1.0f / (scale_factor*10));
		// Add tree to trees array
		this->trees.emplace_back(tree);
		this->hitboxes.emplace_back(*tree, min_hitbox_y, max_hitbox_y);

	}
}

WorldTile::~WorldTile()
{
	for (Rock* const& rock : this->rocks) {
		delete rock;
	}
	for (Tree* const& tree : this->trees) {
		delete tree;
	}

    for (RockB* const& rock : this->rocksB) {
        delete rock;
    }
}

const std::vector<glm::vec3>& WorldTile::getVertices() const
{
	static const std::vector<glm::vec3> vertices = {
			glm::vec3(0.0f, 0.0f, 0.0f), // bottom-left
			glm::vec3(1.0f, 0.0f, 0.0f), // bottom-right
			glm::vec3(1.0f, 0.0f, 1.0f), // top-right
			glm::vec3(0.0f, 0.0f, 1.0f)  // top-left
	};

	return vertices;
}

GLuint WorldTile::getVAO() {
	static const std::vector<GLuint> elements {
			// first triangle (ACTUALLY is counterclockwise - negative-Z axis)
			3, // top-left
			1, // bottom-right
			0, // bottom-left
			// second triangle
			3, // top-left
			2, // top-right
			1  // bottom-right
	};

	static const std::vector<glm::vec3> normals {
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
	};

	static GLuint vao;
	static bool vao_init = false;

	if (!vao_init) {
		// only initialize vao once for all instances
		vao = this->initVertexArray(
				this->getVertices(),
				elements,
				normals
		);
		vao_init = true;
	}

	return vao;
}

const int WorldTile::getColorType() {
	return COLOR_TILE;
}

// tests for x-z collision between the specified box and any child entities.
// does NOT test if the box exists at all in the tile space.
bool WorldTile::collidesWith(const HitBox2d &box) const
{
	for (const HitBox2d& hb : this->hitboxes) {
		if (hb.collidesWith(box)) {
			return true;
		}
	}
	return false;
}
