#include "GLProgramUtils.h"
#include "Model.h"
#include "Scene.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Model::Model(char const* texture, const std::vector<VertexAttribute>& vertexSpec, float const* vertices, size_t numVertices)
    : vertexSpec_(vertexSpec), numVerts_(numVertices), vertexSize_(0), pos({ 0, 0, 0 }), scale({ 1, 1, 1 }), texturePath_(texture)
{
    if (!setupTexture(texturePath_, &texture_)) {
        throw "Couldn't set up texture.";
    }

	// Calculate the size of a vertex (not going to handle every single case since I'm not using most of them
	for (auto& attr : vertexSpec_) {
		switch (attr.type) {
		case GL_FLOAT:
		case GL_UNSIGNED_INT:
		case GL_INT:
			vertexSize_ += attr.size * 4;
			break;
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
			vertexSize_ += attr.size * 2;
			break;
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			vertexSize_ += attr.size;
			break;
		}
	}

    size_t numFloats = numVerts_ * vertexSize_ / sizeof(float);
	vertices_ = new float[numFloats];
	std::copy(vertices, vertices + numFloats, vertices_);

    // Generate and bind the vertex array object so that we only have to make this the active one later to draw
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Fill a buffer with the vertices for this object
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertexSize_ * numVerts_, vertices_, GL_STATIC_DRAW);

    // Set up the vertex specification
    setVertexAttributes(&vertexSpec_[0], vertexSpec_.size());

    glBindVertexArray(0);
}

Model::Model(const Model & other)
	: Model(other.texturePath_, other.vertexSpec_, other.vertices_, other.numVerts_)
{
}

Model::Model(Model && other)
	: vertexSpec_(std::move(other.vertexSpec_)), vertices_(other.vertices_), numVerts_(other.numVerts_), vao_(other.vao_), vbo_(other.vbo_), texture_(other.texture_),
      pos(other.pos), scale(other.scale)
{
	other.vertices_ = nullptr;
    other.vao_ = 0;
    other.vbo_ = 0;
    other.texture_ = 0;
}

Model::~Model()
{
	delete[] vertices_;
    glDeleteTextures(1, &texture_);
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

Model & Model::operator=(Model rhs)
{
	swap(*this, rhs);

	return *this;
}

void Model::update(float deltaTime)
{
    // Turns out I don't really need this... whoops
}

void Model::draw(Scene const& scene)
{
    glm::mat4x4 mvp(scene.getCamera().getViewProjectionMatrix() * glm::translate(pos) * glm::scale(scale));
    Shader const& modelShader = scene.getShaders().find("model")->second;

    modelShader.makeCurrent();
    glBindTexture(GL_TEXTURE_2D, texture_);
    
    glBindVertexArray(vao_);
    glUniformMatrix4fv(modelShader.getUniforms().find("mvp")->second, 1, GL_FALSE, glm::value_ptr(mvp));

    glDrawArrays(GL_TRIANGLES, 0, numVerts_);
}
