#include "GLProgramUtils.h"
#include "Drawable3D.h"
#include "Scene.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <cstring>

Drawable3D::Drawable3D(char const* texture, const std::vector<VertexAttribute>& vertexSpec, void * vertices,
                       size_t numVertices, bool copyVerts)
    : Drawable3DContainer(), vertexSpec_(vertexSpec), numVerts_(numVertices), vertexSize_(0), texturePath_(texture)
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

    if (copyVerts)
    {
        size_t numFloats = numVerts_ * vertexSize_;
        vertices_ = ::malloc(numVerts_ * vertexSize_);
        memcpy(vertices_, vertices, numVerts_ * vertexSize_);
    }
    else
    {
        vertices_ = vertices;
    }

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

Drawable3D::~Drawable3D()
{
    std::free(vertices_);
    glDeleteTextures(1, &texture_);
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
}

void Drawable3D::update(float deltaTime)
{
    // Turns out I don't really need this... whoops
}

void Drawable3D::draw(Scene const& scene)
{
    glm::mat4 mvp(scene.getCamera().getViewProjectionMatrix() * glm::translate(pos) * glm::scale(scale));
    Shader const& modelShader = scene.getShaders().find("model")->second;

    modelShader.makeCurrent();
    glBindTexture(GL_TEXTURE_2D, texture_);
    
    glBindVertexArray(vao_);
    glUniformMatrix4fv(modelShader.getUniforms().find("mvp")->second, 1, GL_FALSE, glm::value_ptr(mvp));

    glDrawArrays(GL_TRIANGLES, 0, numVerts_);
}
