#pragma once
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "GLProgramUtils.h"

class Model {
public:

	/**
	 * Creates a new 3D model with the specified vertex format and vertices.
	 * @param vertexSpec  A list of vertex attributes describing the memory layout of each vertex.
	 * @param vertices    The list of vertices making up this model. The model will free this on destruction.
	 * @param numVertices The number of vertices in the list.
	 */
	Model(std::vector<VertexAttribute> vertexSpec, float* vertices, size_t numVertices);

	Model(const Model& other);
	Model(Model&& other);

	~Model();

	Model& operator=(Model rhs);

	friend void swap(Model& left, Model& right) noexcept {
		using std::swap;

		swap(left.vertexSpec_, right.vertexSpec_);
		swap(left.vertices_, right.vertices_);
		swap(left.numVerts_, right.numVerts_);
		swap(left.vertexSize_, right.vertexSize_);
	}

private:
	// Drawing info
	std::vector<VertexAttribute> vertexSpec_;
	float* vertices_;
	size_t numVerts_;
	size_t vertexSize_;

	// Transform info
	glm::vec3 pos_;
	glm::vec3 scale_;
	glm::quat orientation_;
};