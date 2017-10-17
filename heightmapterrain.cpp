#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

#include "entity.hpp"
#include "constants.hpp"
#include "heightmapterrain.hpp"

HeightMapTerrain::HeightMapTerrain(
	const GLuint &shader_program,
	const std::string& map_path,
	Entity* parent
) : Entity(parent)
{
	static const int requested_channels = 3;

	int map_width, map_height, channels;
	// Read image data using 3 channels (red, green, blue)
	float* image_data = stbi_loadf(map_path.c_str(), &map_width, &map_height, &channels, requested_channels);

	if (!image_data) {
		throw std::runtime_error("Failed to load image " + map_path + ": " + stbi_failure_reason());
	}

	// BEGIN STEP 1: Points taken directly from pixel color values.
	// This is independent of user-provided skip_size so it only happens
	// once for a given image.

	for (int i = 0, limit = map_width * map_height * requested_channels; i < limit; i += requested_channels) {
		// average of color values becomes y value (height); values are between 0 and 1.
		// averaging compensates for non-greyscale images
		float height = 0;
		for (int j = 0; j < requested_channels; j++) {
			height += image_data[i + j];
		}
		height /= requested_channels;

		// use x and y values of 2d image to get x and z in 3d space, centered around origin
		int pixel_number = i / requested_channels;
		float x = (pixel_number % map_width) - (map_width / 2.0f);
		float z = (pixel_number / map_width) - (map_height / 2.0f);

		this->vertices_step_1.emplace_back(x, height, z);
	}

	// Free image data memory since we're not using it anymore
	stbi_image_free(image_data);

	std::vector<GLuint> elements_step_1;
	HeightMapTerrain::createElements(map_width, map_height, &elements_step_1);
	this->vao_step_1 = Entity::initVertexArray(
			shader_program,
			this->vertices_step_1,
			elements_step_1,
			&this->vertices_buffer_step_1,
			&this->element_buffer_step_1
	);

	// END STEP 1

	// defer generation of vertices for steps 2-4 until setSkipSize is called
	this->awaiting_derived_vertices_initialization = true;

	// These are necessary before we call the generateDerivedVertices method
	this->shader_program = shader_program;
	this->map_width = map_width;
	this->map_height = map_height;

	this->selected_step = 1;

	// since the model y coordinates are in a range between 0 and 1,
	// we should scale them to look more pronounced.
	// arbitrarily choosing average of width and height divided by 4
	float y_scale = (map_width + map_height) / 8.0f;
	this->base_scale = glm::scale(this->base_scale, glm::vec3(1.0f, y_scale, 1.0f));

	this->draw_mode = GL_TRIANGLES;
}

HeightMapTerrain::~HeightMapTerrain()
{
	glDeleteBuffers(1, &this->vertices_buffer_step_1);
	glDeleteBuffers(1, &this->element_buffer_step_1);
	glDeleteVertexArrays(1, &this->vao_step_1);

	this->deleteDerivedBuffers();
}

const std::vector<glm::vec3>& HeightMapTerrain::getVertices()
{
	switch (this->selected_step) {
		case 1:
			return this->vertices_step_1;
		case 2:
			return this->vertices_step_2;
		case 3:
			return this->vertices_step_3;
		case 4:
			return this->vertices_step_4;
		default:
			return this->vertices_step_1;
	}
}

GLuint HeightMapTerrain::getVAO()
{
	switch (this->selected_step) {
		case 1:
			return this->vao_step_1;
		case 2:
			return this->vao_step_2;
		case 3:
			return this->vao_step_3;
		case 4:
			return this->vao_step_4;
		default:
			return this->vao_step_1;
	}
}

const int HeightMapTerrain::getColorType()
{
	return COLOR_HEIGHT;
}

const glm::mat4& HeightMapTerrain::getBaseScale()
{
	return this->base_scale;
}

void HeightMapTerrain::setSkipSize(const int& skip_size)
{
	this->generateDerivedVertices(skip_size);
}

void HeightMapTerrain::selectStep(const int &step_number)
{
	if (step_number < 1 || step_number > 4) {
		throw std::runtime_error("Step number must be in the range 1-4.");
	}

	if (step_number > 1 && this->awaiting_derived_vertices_initialization) {
		throw std::runtime_error("Cannot render steps 2-4 until skip size has been set.");
	}

	this->selected_step = step_number;
	std::cout << "Now displaying step " << step_number << "!\n";
}

void HeightMapTerrain::generateDerivedVertices(const int& skip_size)
{
	// Tell the GPU to get rid of any memory we've already allocated for this data
	this->deleteDerivedBuffers();

	// Clear previously generated vertices
	this->vertices_step_2.clear();
	this->vertices_step_3.clear();
	this->vertices_step_4.clear();

	// BEGIN STEP 2: Points reduced via skip interval

	for (int y = 0; y < this->map_height; y += skip_size) {
		int offset = y * this->map_width;
		for (int x = 0; x < this->map_width; x += skip_size) {
			this->vertices_step_2.push_back(this->vertices_step_1[offset + x]);

			int x_remaining = this->map_width - x;
			if (x_remaining > 1 && x_remaining <= skip_size) {
				this->vertices_step_2.push_back(this->vertices_step_1[offset + this->map_width - 1]);
			}
		}

		int y_remaining = this->map_height - y;
		if (y_remaining > 1 && y_remaining <= skip_size) {
			offset = (this->map_height - 1) * this->map_width;
			for (int x = 0; x < this->map_width; x += skip_size) {
				this->vertices_step_2.push_back(this->vertices_step_1[offset + x]);

				int x_remaining = this->map_width - x;
				if (x_remaining > 1 && x_remaining <= skip_size) {
					this->vertices_step_2.push_back(this->vertices_step_1[offset + this->map_width - 1]);
				}
			}
		}
	}

	// sampled vertices should contain first and last vertex in each row/column,
	// regardless of skip interval
	int reduced_width = map_width / skip_size + (map_width % skip_size == 0 ? 1 : 2);
	int reduced_height = map_height / skip_size + (map_height % skip_size == 0 ? 1 : 2);

	std::vector<GLuint> elements_step_2;
	HeightMapTerrain::createElements(reduced_width, reduced_height, &elements_step_2);
	this->vao_step_2 = Entity::initVertexArray(
			this->shader_program,
			this->vertices_step_2,
			elements_step_2,
			&this->vertices_buffer_step_2,
			&this->element_buffer_step_2
	);

	// END STEP 2

	// BEGIN STEP 3: Missing points from step 2 spline-interpolated along x-axis



	// END STEP 3

	// BEGIN STEP 4: Remaining missing points from step 3 spline-interpolated along z-axis



	// END STEP 4

	this->awaiting_derived_vertices_initialization = false;
}

void HeightMapTerrain::deleteDerivedBuffers()
{
	glDeleteBuffers(1, &this->vertices_buffer_step_2);
	glDeleteBuffers(1, &this->element_buffer_step_2);
	glDeleteVertexArrays(1, &this->vao_step_2);

	glDeleteBuffers(1, &this->vertices_buffer_step_3);
	glDeleteBuffers(1, &this->element_buffer_step_3);
	glDeleteVertexArrays(1, &this->vao_step_3);

	glDeleteBuffers(1, &this->vertices_buffer_step_4);
	glDeleteBuffers(1, &this->element_buffer_step_4);
	glDeleteVertexArrays(1, &this->vao_step_4);
}

void HeightMapTerrain::createElements(
	const int& width,
	const int& height,
	std::vector<GLuint>* const elements
) {
	if (elements == nullptr) {
		throw std::runtime_error("Elements vector must not be null.");
	}

	// since we know our terrain comes from an image, i.e. a fully-filled 2d array,
	// we can create an element array using an uncomplicated algorithm which creates
	// two triangles to connect each arrangement of 4 adjacent vertices.
	for (int y = height - 1; y--; ) {
		int offset = y * width;
		for (int x = width - 1; x--; ) {
			// first triangle
			elements->emplace_back(offset + x); // top-left
			elements->emplace_back(offset + x + 1); // top-right
			elements->emplace_back(offset + width + x); // bottom-left

			// second triangle
			elements->emplace_back(offset + width + x); // bottom-left
			elements->emplace_back(offset + x + 1); // top-right
			elements->emplace_back(offset + width + x + 1); // bottom-right
		}
	}
}