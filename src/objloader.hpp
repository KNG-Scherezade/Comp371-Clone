#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <glm/glm.hpp>
#include <vector>

void loadOBJ(
		const char* path,
		std::vector<glm::vec3>* const& out_vertices,
		std::vector<glm::vec3>* const& out_normals,
		std::vector<glm::vec2>* const& out_uvs,
		std::vector<unsigned int>* const& out_elements
);

#endif // OBJLOADER_H
