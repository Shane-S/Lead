#include "TestModelVerts.h"

float cube[] = { 0 };
VertexAttribute cubeSpec[] = {
	{ VertexAttribPos, 3, GL_FLOAT, GL_FALSE, 32, 0 },
    { VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 32, 12 },
	{ VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 32, 24 },
};
char const* cubeTexture = "cube.png";
size_t cubeVertSize = sizeof(float) * 8;
size_t numCubeVerts = 36;
VertexAttribute* cubeVertSpec = &cubeSpec[0];
float* cubeVerts = &cube[0];

float ship[] = {
	#include "vikingVertsHeavy.txt"
};
VertexAttribute shipSpec[] = {
	{ VertexAttribPos, 3, GL_FLOAT, GL_FALSE, 32, 0 },
	{ VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 32, 12 },
	{ VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 32, 24 },
};
char const* shipTexture = "assets/VikingDiff.png";
VertexAttribute* shipVertSpec = &shipSpec[0];
size_t shipVertSize = sizeof(float) * 8;
size_t numShipVerts = sizeof(ship) / shipVertSize;