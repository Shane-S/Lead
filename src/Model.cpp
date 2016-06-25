#include "Model.h"

Model::Model(char const* texture, std::vector<VertexAttribute> vertexSpec, float const* vertices, size_t numVertices)
	: vertexSpec_(std::move(vertexSpec)), numVerts_(numVertices), vertexSize_(0)
{
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

	vertices_ = new float[numVerts_];
	std::copy(vertices, vertices + numVerts_, vertices_);
}

Model::Model(const Model & other)
	: vertexSpec_(other.vertexSpec_), numVerts_(other.numVerts_), vertices_(new float[other.numVerts_]), vertexSize_(other.vertexSize_)
{
	std::copy(other.vertices_, other.vertices_ + numVerts_, vertices_);
}

Model::Model(Model && other)
	: vertexSpec_(std::move(other.vertexSpec_)), vertices_(other.vertices_), numVerts_(other.numVerts_)
{
	other.vertices_ = nullptr;
}

Model::~Model()
{
	delete[] vertices_;
}

Model & Model::operator=(Model rhs)
{
	swap(*this, rhs);

	return *this;
}
