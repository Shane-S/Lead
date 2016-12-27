#include "Scene.h"
#include "FloorPlan.h"

FloorPlan::FloorPlan(pb_hashmap *room_specs, pb_sq_house_house_spec* house_spec)
{
    building_ = pb_sq_house(house_spec, room_specs);
    if (!building_)
    {
        throw "Couldn't create building!";
    }
}

void FloorPlan::update(float dt)
{
    // Starting to wonder why I even bothered with this method
}

void FloorPlan::draw(Scene const& scene)
{

}

FloorPlan::~FloorPlan()
{
    pb_building_free(building_, pb_sq_house_free_building, pb_sq_house_free_floor, pb_sq_house_free_room);
}
