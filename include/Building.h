#pragma once

#include <map>
#include <string>
#include <pb/extrusion.h>
#include <GL/glew.h>
#include "Drawable3DContainer.h"
#include "FloorPlan.h"

class Building : public Drawable3DContainer {
public:
    Building(FloorPlan const& fp, std::map<std::string, GLuint>& textures);

private:
    pb_extruded_floor** building_;
    size_t numFloors_;
};
