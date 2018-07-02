#include <vector>
#include <string>
#include <stdexcept>

#include <glm/glm.hpp>
#include <src/vendor/tiny_obj_loader.cc>

#include "objloader.hpp"

void loadOBJ(
		const char* path,
		std::vector<glm::vec3>* const& out_vertices,
		std::vector<glm::vec3>* const& out_normals,
		std::vector<glm::vec2>* const& out_uvs,
		std::vector<unsigned int>* const& out_elements
) {
	// .obj loading code adapted from sample usage code at:
	// https://github.com/syoyo/tinyobjloader/blob/b434c2497fcb52aa1497b84aa8aeb12bb590492d/README.md#example-code

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

	if (!err.empty()) {
		throw std::runtime_error("Obj load failed: " + err);
	}

	if (shapes.empty()) {
		throw std::runtime_error("Obj must contain shape!");
	}

	if (!ret) {
		throw std::runtime_error("Obj load failed.");
	}

	// populate vertices
	for (size_t i = 0, len = attrib.vertices.size(); i < len; i += 3) {
		out_vertices->emplace_back(
				attrib.vertices[i],
				attrib.vertices[i + 1],
				attrib.vertices[i + 2]
		);
	}

	// populate normals
	for (size_t i = 0, len = attrib.normals.size(); i < len; i+= 3) {
		out_normals->emplace_back(
				attrib.normals[i],
				attrib.normals[i + 1],
				attrib.normals[i + 2]
		);
	}

	// populate uvs
	for (size_t i = 0, len = attrib.texcoords.size(); i < len; i+= 2) {
		out_uvs->emplace_back(attrib.texcoords[i], attrib.texcoords[i + 1]);
	}

	// populate elements
	tinyobj::shape_t shape = shapes[0];
	for (const tinyobj::index_t& index : shape.mesh.indices) {
		out_elements->emplace_back(index.vertex_index);
	}
}
