#pragma once
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <memory>
#include "GLProgramUtils.h"
#include "Camera.h"
#include "Drawable3DContainer.h"

class Scene;

class Drawable3D : public Drawable3DContainer {
public:

	/**
	 * Creates a new 3D model with the specified vertex format and vertices.
	 * @param texture     The path to the texture to load with this model. The model will call glDeleteTextures on this
	 *                    texture when it is destructed.
	 * @param vertexSpec  A list of vertex attributes describing the memory layout of each vertex.
	 * @param vertices    The list of vertices making up this model.
	 * @param numVertices The number of vertices in the list.
	 * @param copyVerts   Whether to copy the vertices from the given list. If false, the Drawable3D instance assumes
	 *                    that this pointer is malloc'd and will assume its ownership.
	 */
    Drawable3D(char const* texture, const std::vector<VertexAttribute>& vertexSpec, void * vertices,
                           size_t numVertices, bool copyVerts);

	/**
	 * Creates a new 3D model with the specified vertex format, vertices, and pre-loaded texture.
	 *
	 * @param texture     The GLuint identifying the texture to use for this model. The model will NOT call
	 *                    glDeleteTextures for this texture when it is destroyed.
	 * @param vertexSpec  A list of vertex attributes describing the memory layout of each vertex.
	 * @param vertices    The list of vertices making up this model.
	 * @param numVertices The number of vertices in the list.
	 * @param copyVerts   Whether to copy the vertices from the given list. If false, the Drawable3D instance assumes
	 *                    that this pointer is malloc'd and will assume its ownership.
	 */
	Drawable3D(GLuint texture, const std::vector<VertexAttribute>& vertexSpec, void* vertices,
			   size_t numVertices, bool copyVerts);

	Drawable3D(const Drawable3D& other) = delete;
	Drawable3D(Drawable3D&& other) = delete;

	~Drawable3D();

	Drawable3D& operator=(Drawable3D & rhs) = delete;

    /** 
     * Computes the model's new position and orientation based on velocity and angular velocity.
     */
    void update(float deltaTime);

    /**
     * Draws the model.
     */
    void draw(Scene const& scene);

private:
	// Drawing info
	std::vector<VertexAttribute> vertexSpec_;
	void * vertices_;
	size_t numVerts_;
	size_t vertexSize_;

	bool shouldDeleteTexture_;
    GLuint texture_;
    GLuint vao_;
    GLuint vbo_;
};