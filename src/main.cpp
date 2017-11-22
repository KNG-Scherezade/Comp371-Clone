//example skeleton code
//modified from http://learnopengl.com/

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#define GLFW_INCLUDE_NONE // don't include deprecated gl headers on macOS
#include <GLFW/glfw3.h>	// include GLFW helper library

#include <iostream>
#include <chrono>
#include <ratio>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "glsetup.hpp"       // include gl context setup function
#include "shaderprogram.hpp" // include the shader program compiler
#include "src/entities/Entity.hpp"
#include "src/entities/World.hpp"
#include "src/entities/Player.hpp"
#include "constants.hpp"

World* world;

// Camera constants
const glm::vec3 up = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
// pitch and yaw stored in degrees for clarity, but must be converted
// to radians to work well with glm
const float initial_pitch = -65.0f;
const float initial_yaw = -90.0f;
const float max_pitch = 89.0f;
const float min_pitch = -89.0f;
const float first_person_pitch = 45.0f;
const float max_follow_distance = 300.0f;

// Camera variables
float pitch = initial_pitch;
float yaw = initial_yaw;

// Projection variables, to be set by framebufferSizeCallback
int framebuffer_width = 0;
int framebuffer_height = 0;

float getPlayerScaleCoefficient()
{
	glm::vec3 scale_vec = world->getPlayer()->getScale();
	return (scale_vec.x + scale_vec.y + scale_vec.z) / 3.0f;
}

glm::vec3 getViewDirection() {
	return glm::vec3(
			(float)(cos(glm::radians(yaw)) * cos(glm::radians(pitch))),
			(float)sin(glm::radians(pitch)),
			(float)(sin(glm::radians(yaw)) * cos(glm::radians(pitch)))
	);
}

glm::vec3 getFollowVector() {
	return glm::normalize(getViewDirection()) *
			max_follow_distance *
			// scale follow distance according to player size so player always
			// takes up same proportion of screen for a given viewing angle
			getPlayerScaleCoefficient() *
			// The lower the viewing angle, the shorter the follow distance -
			// to accommodate for less space near terrain. At a specified high pitch,
			// the third-person camera becomes first-person.
			std::max(
				0.0f,
				(1 - (pitch - min_pitch) / (first_person_pitch - min_pitch))
			);
}

// returns [0, 1) float representing progress through day/night cycle,
// based on actual current system time (but with shortened day length)
float getDayProgress() {
	// our "day" is actually 10 minutes long
	static const double day_seconds = 10 * 10;

	double now = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
	).count();
	double progress = now / day_seconds;
	// wrap into [0, 1) range
	return float((long)progress % (long)day_seconds / day_seconds);
}

bool isKeyPressed(GLFWwindow* const& window, const int& key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

// controls that should be polled at every frame and read
// continuously / in combination
void pollContinuousControls(GLFWwindow* window) {
	Player* player = world->getPlayer();
	// move forward
	if (isKeyPressed(window, GLFW_KEY_W) || isKeyPressed(window, GLFW_KEY_UP)) {
		player->moveForward(getViewDirection(), up, PLAYER_MOVEMENT_SPEED);
		world->checkPosition();
	}
	// move back
	if (isKeyPressed(window, GLFW_KEY_S) || isKeyPressed(window, GLFW_KEY_DOWN)) {
		player->moveBack(getViewDirection(), up, PLAYER_MOVEMENT_SPEED);
		world->checkPosition();
	}
	// move left
	if (isKeyPressed(window, GLFW_KEY_A) || isKeyPressed(window, GLFW_KEY_LEFT)) {
		player->moveLeft(getViewDirection(), up, PLAYER_MOVEMENT_SPEED);
		world->checkPosition();
	}
	// move right
	if (isKeyPressed(window, GLFW_KEY_D) || isKeyPressed(window, GLFW_KEY_RIGHT)) {
		player->moveRight(getViewDirection(), up, PLAYER_MOVEMENT_SPEED);
		world->checkPosition();
	}
}

// Is called whenever a key is pressed/released via GLFW
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// ignore key release actions for now
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
			case GLFW_KEY_GRAVE_ACCENT:
				world->toggleAxes();
				break;
			case GLFW_KEY_BACKSPACE:
				// Reset camera
				pitch = initial_pitch;
				yaw = initial_yaw;
				break;
			case GLFW_KEY_ESCAPE:
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;
			default:
				break;
		}
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	// Thanks to https://learnopengl.com/#!Getting-started/Camera for helping
	// me think about camera movement!

	static bool was_mouse_captured = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;

	static double last_xpos = xpos;
	static double last_ypos = ypos;

	static float sensitivity = 0.2f;

	bool is_mouse_captured = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
	if (!is_mouse_captured) {
		was_mouse_captured = false;
		// we don't want to handle camera movement if the mouse isn't captured
		return;
	}
	if (!was_mouse_captured) {
		// don't jerk the camera around if we're recapturing the mouse
		last_xpos = xpos;
		last_ypos = ypos;
	}
	was_mouse_captured = true;

	auto x_diff = sensitivity * (float)(xpos - last_xpos);
	auto y_diff = sensitivity * (float)(last_ypos - ypos);
	last_xpos = xpos;
	last_ypos = ypos;

	yaw += x_diff;
	pitch += y_diff;

	// set some vertical limits to avoid weird behavior
	if (pitch > max_pitch) {
		pitch = max_pitch;
	} else if (pitch < min_pitch) {
		pitch = min_pitch;
	}
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	// Update the viewport dimensions
	glViewport(0, 0, width, height);

	// update projection matrix variables to maintain aspect ratio
	framebuffer_width = width;
	framebuffer_height = height;
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	GLFWwindow* window = nullptr;
	setupGlContext(WIDTH, HEIGHT, APP_NAME, &window);

	// Set the required callback functions
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Set up viewport and projection matrix, which will be updated whenever the framebuffer resizes.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	framebufferSizeCallback(window, width, height);

	bool shader_program_ok;
	GLuint shader_program = prepareShaderProgram(
		"../shaders/vertex.glsl",
		"../shaders/fragment.glsl",
		&shader_program_ok
	);
	if (!shader_program_ok) {
		return -1;
	}

	world = new World(shader_program);

	// default sun ray points down (noon)
    const glm::vec3 default_sun_direction(0.0f, -1.0f, 0.0f);
	const glm::vec3 sunlight(0.9f, 0.87f, 0.8f);
	const glm::vec3 moonlight(0.4f, 0.4f, 0.4f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		static glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
		static glm::vec3 y_axis(0.0f, 1.0f, 0.0f);

		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		pollContinuousControls(window);

		float day_progress = getDayProgress();

		// Render
		// Clear the colorbuffer (sky color)
		float sky_coefficient = 2.0f + std::abs(std::fmod(day_progress * 2.0f, 2.0f) - 1.0f) * 4.0f;
		glClearColor(
			sky_coefficient * 0.12f,
			sky_coefficient * 0.19f,
			sky_coefficient * 0.23f,
			1.0f
		);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 player_position = world->getPlayer()->getPosition();
		glm::mat4 view_matrix = glm::lookAt(player_position - getFollowVector(), player_position, up);

		float player_scale = getPlayerScaleCoefficient();
		glm::mat4 projection_matrix = glm::perspective(
			45.0f,
			(GLfloat)framebuffer_width / (GLfloat)framebuffer_height,
			30.0f * player_scale,
			1500.0f * player_scale
		);

		bool show_moon = day_progress > 0.25 && day_progress <= 0.75;
		Light current_celestial_ray(
			// rotate sun (or moon) toward the west
			glm::rotateZ(
				default_sun_direction,
				float(M_PI * 2 * day_progress + (show_moon ? M_PI : 0))
			),
			show_moon ? moonlight : sunlight
		);

		world->draw(view_matrix, projection_matrix, current_celestial_ray);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	delete world;

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
