#pragma once
#include <pb/floor_plan.h>
#include <pb/sq_house.h>
#include "Drawable.h"

class FloorPlan : public Drawable
{
public:
    FloorPlan(pb_hashmap* roomSpecs, pb_sq_house_house_spec* houseSpec);

    virtual void draw(Scene const& scene) override;
    virtual void update(float dt) override;

    pb_building* getBuilding() const {return building_;}

    ~FloorPlan();
private:
    pb_building* building_;
};
