#include "TestModelVerts.h"

static float plane[] = {
   -0.5f,     -0.5f,     0,    0,    0,    1.0f,    0, 1,
    0.5f,      0.5f,     0,    0,    0,    1.0f,    1, 0,
   -0.5f,      0.5f,     0,    0,    0,    1.0f,    0, 0,
    0.5f,     -0.5f,     0,    0,    0,    1.0f,    1, 1,
    0.5f,      0.5f,     0,    0,    0,    1.0f,    1, 0,
   -0.5f,     -0.5f,     0,    0,    0,    1.0f,    0, 1
};
float* planeVerts = &plane[0];
char const* planeTexture = "assets/plane.bmp";
VertexAttribute planeSpec[] = {
    { VertexAttribPos, 3, GL_FLOAT, GL_FALSE, 32, 0 },
    { VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 32, 12 },
    { VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 32, 24 },
};
VertexAttribute* planeVertSpec = &planeSpec[0];
size_t numPlaneAttrs = 3;
size_t planeVertSize = sizeof(float) * 8;
size_t numPlaneVerts = 6;

static float cube[] = {
	// Data layout for each line below is:
    // positionX, positionY, positionZ,  normalX, normalY, normalZ, u,     v
    0.5f,     -0.5f,     -0.5f,          1.0f,    0.0f,    0.0f,    1.f/3, 2.f/3,
	0.5f,      0.5f,     -0.5f,          1.0f,    0.0f,    0.0f,    1.f/3, 1,
	0.5f,     -0.5f,      0.5f,          1.0f,    0.0f,    0.0f,    0,     2.f/3,
	0.5f,     -0.5f,      0.5f,          1.0f,    0.0f,    0.0f,    0,     2.f/3,
	0.5f,      0.5f,     -0.5f,          1.0f,    0.0f,    0.0f,    1.f/3, 1,
	0.5f,      0.5f,      0.5f,          1.0f,    0.0f,    0.0f,    0,     1,

	0.5f,      0.5f,     -0.5f,          0.0f,    1.0f,    0.0f,    2.f/3, 1,
   -0.5f,      0.5f,     -0.5f,          0.0f,    1.0f,    0.0f,    1.f/3, 1,
	0.5f,      0.5f,      0.5f,          0.0f,    1.0f,    0.0f,    2.f/3, 2.f/3,
	0.5f,      0.5f,      0.5f,          0.0f,    1.0f,    0.0f,    2.f/3, 2.f/3,
   -0.5f,      0.5f,     -0.5f,          0.0f,    1.0f,    0.0f,    1.f/3, 1,
   -0.5f,      0.5f,      0.5f,          0.0f,    1.0f,    0.0f,    1.f/3, 2.f/3,

   -0.5f,      0.5f,     -0.5f,         -1.0f,    0.0f,    0.0f,    2.f/3, 1,
   -0.5f,     -0.5f,     -0.5f,         -1.0f,    0.0f,    0.0f,    2.f/3, 2.f/3,
   -0.5f,      0.5f,      0.5f,         -1.0f,    0.0f,    0.0f,    1,     1,
   -0.5f,      0.5f,      0.5f,         -1.0f,    0.0f,    0.0f,    1,     1,
   -0.5f,     -0.5f,     -0.5f,         -1.0f,    0.0f,    0.0f,    2.f/3, 2.f/3,
   -0.5f,     -0.5f,      0.5f,         -1.0f,    0.0f,    0.0f,    1,     2.f/3,

   -0.5f,     -0.5f,     -0.5f,          0.0f,    -1.0f,   0.0f,    0,     1.f/3,
	0.5f,     -0.5f,     -0.5f,          0.0f,    -1.0f,   0.0f,    1.f/3, 1.f/3,
   -0.5f,     -0.5f,      0.5f,          0.0f,    -1.0f,   0.0f,    0,     2.f/3,
   -0.5f,     -0.5f,      0.5f,          0.0f,    -1.0f,   0.0f,    0,     2.f/3,
	0.5f,     -0.5f,     -0.5f,          0.0f,    -1.0f,   0.0f,    1.f/3, 1.f/3,
	0.5f,     -0.5f,      0.5f,          0.0f,    -1.0f,   0.0f,    1.f/3, 2.f/3,

	0.5f,      0.5f,      0.5f,          0.0f,     0.0f,   1.0f,    2.f/3, 2.f/3,
   -0.5f,      0.5f,      0.5f,          0.0f,     0.0f,   1.0f,    1.f/3, 2.f/3,
	0.5f,     -0.5f,      0.5f,          0.0f,     0.0f,   1.0f,    2.f/3, 1.f/3,
	0.5f,     -0.5f,      0.5f,          0.0f,     0.0f,   1.0f,    2.f/3, 1.f/3,
   -0.5f,      0.5f,      0.5f,          0.0f,     0.0f,   1.0f,    1.f/3, 2.f/3,
   -0.5f,     -0.5f,      0.5f,          0.0f,     0.0f,   1.0f,    1.f/3, 1.f/3,

	0.5f,     -0.5f,     -0.5f,          0.0f,     0.0f,  -1.0f,    2.f/3, 1.f/3,
   -0.5f,     -0.5f,     -0.5f,          0.0f,     0.0f,  -1.0f,    1,     1.f/3,
	0.5f,      0.5f,     -0.5f,          0.0f,     0.0f,  -1.0f,    2.f/3, 2.f/3,
	0.5f,      0.5f,     -0.5f,          0.0f,     0.0f,  -1.0f,    2.f/3, 2.f/3,
   -0.5f,     -0.5f,     -0.5f,          0.0f,     0.0f,  -1.0f,    1,     1.f/3,
   -0.5f,      0.5f,     -0.5f,          0.0f,     0.0f,  -1.0f,    1,     2.f/3 };
VertexAttribute cubeSpec[] = {
	{ VertexAttribPos, 3, GL_FLOAT, GL_FALSE, 32, 0 },
    { VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 32, 12 },
	{ VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 32, 24 },
};
size_t numCubeAttrs = sizeof(cubeSpec) / sizeof(VertexAttribute);
char const* cubeTexture = "assets/cube.png";
size_t cubeVertSize = sizeof(float) * 8;
size_t numCubeVerts = 36;
VertexAttribute* cubeVertSpec = &cubeSpec[0];
float* cubeVerts = &cube[0];

static float ship[] = {
	#include "vikingVertsHeavy.txt"
};
VertexAttribute shipSpec[] = {
	{ VertexAttribPos, 3, GL_FLOAT, GL_FALSE, 32, 0 },
	{ VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 32, 12 },
	{ VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 32, 24 },
};
size_t numShipAttrs = sizeof(shipSpec) / sizeof(VertexAttribute);
char const* shipTexture = "assets/VikingDiff.png";
VertexAttribute* shipVertSpec = &shipSpec[0];
size_t shipVertSize = sizeof(float) * 8;
size_t numShipVerts = sizeof(ship) / shipVertSize;
float* shipVerts = &ship[0];