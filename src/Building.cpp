#include <pb/floor_plan.h>
#include <pb/extrusion.h>
#include <pb/simple_extruder.h>
#include <pb/util/geom/types.h>

#include <cstring>

#include "Building.h"
#include "Drawable3D.h"

Building::Building(FloorPlan const& fp, std::map<std::string, GLuint>& textures)
{
    pb_building* b = fp.getBuilding();
    pb_extruded_floor** floors = pb_extrude_building(fp.getBuilding(), 2.f, 1.5f, 0.7f,
                                                     pb_simple_door_extruder, pb_simple_window_extruder,
                                                     NULL, NULL);

    std::vector<VertexAttribute> spec;
    spec.push_back({ VertexAttribPos, 3, GL_FLOAT, GL_FALSE, 32, 0 });
    spec.push_back({ VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 32, 12 });
    spec.push_back({ VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 32, 24 });

    GLuint wallTexture = textures["Stucco wall"];
    GLuint exteriorWallTexture = textures["Wood siding"];

    // Take ownership of all vertices and free the lists as we go
    for (auto i = 0; i < b->num_floors; ++i)
    {
        pb_extruded_floor* f = floors[i];
        for (auto j = 0; j < f->num_rooms; ++j)
        {
            pb_extruded_room* r = f->rooms[j];
            for (auto wall_list_idx = 0; wall_list_idx < r->num_wall_lists; ++wall_list_idx)
            {
                pb_shape3D* wall_list = r->walls[wall_list_idx];
                for (auto wall_shape_idx = 0; wall_shape_idx < r->wall_counts[wall_list_idx]; ++wall_shape_idx)
                {
                    pb_shape3D* wall = wall_list + wall_shape_idx;
                    std::shared_ptr<Drawable3D> wallDrawable(new Drawable3D(wallTexture, spec,
                                                                            wall->tris, wall->num_tris * 3, false));
                    wallDrawable->pos.x = wall->pos.x;
                    wallDrawable->pos.y = wall->pos.y;
                    wallDrawable->pos.z = wall->pos.z;

                    addChild(wallDrawable);
                }
                ::free(wall_list);
            }
            ::free(r->walls);

            for (auto door_idx = 0; door_idx < r->num_doors; ++door_idx)
            {
                pb_shape3D* door = r->doors + door_idx;
                auto doorDrawablePtr = new Drawable3D((GLuint)0, spec, door->tris, door->num_tris * 3, false);
                doorDrawablePtr->setVisible(false);
                doorDrawablePtr->pos.x = door->pos.x;
                doorDrawablePtr->pos.y = door->pos.y;
                doorDrawablePtr->pos.z = door->pos.z;

                std::shared_ptr<Drawable3D> doorDrawable(doorDrawablePtr);
                addChild(doorDrawable);
            }
            ::free(r->doors);

            for (auto window_idx = 0; window_idx < r->num_windows; ++window_idx)
            {
                pb_shape3D* window = r->windows + window_idx;
                auto windowDrawablePtr = new Drawable3D((GLuint)0, spec, window->tris, window->num_tris * 3, false);
                windowDrawablePtr->setVisible(false);
                windowDrawablePtr->pos.x = window->pos.x;
                windowDrawablePtr->pos.y = window->pos.y;
                windowDrawablePtr->pos.x = window->pos.z;

                std::shared_ptr<Drawable3D> windowDrawable(windowDrawablePtr);
                addChild(windowDrawable);
            }
            ::free(r->windows);

            if (r->floor != NULL)
            {
                char const* name = b->floors[i].rooms[j].name;
                bool isAlgorithmGenerated = ::strcmp(name, PB_SQ_HOUSE_HALLWAY) == 0 ||
                                            ::strcmp(name, PB_SQ_HOUSE_STAIRS) == 0;

                std::string strName(name);
                std::string texName = isAlgorithmGenerated ? "Hardwood tile floor" : strName + " floor";

                GLuint texture = textures[texName];

                for (auto floor_shape_idx = 0; floor_shape_idx < r->num_floor_shapes; ++floor_shape_idx)
                {
                    pb_shape3D* shape = r->floor + floor_shape_idx;
                    auto floorDrawablePtr = new Drawable3D(texture, spec, shape->tris, shape->num_tris * 3, false);
                    floorDrawablePtr->pos.x = shape->pos.x;
                    floorDrawablePtr->pos.y = shape->pos.y;
                    floorDrawablePtr->pos.z = shape->pos.z;

                    std::shared_ptr<Drawable3D> floorDrawable(floorDrawablePtr);
                    addChild(floorDrawable);
                }
                free(r->floor);
            }

            if (r->ceiling != NULL)
            {
                GLuint texture = textures["Stucco ceiling"];

                for (auto ceiling_shape_idx = 0; ceiling_shape_idx < r->num_ceiling_shapes; ++ceiling_shape_idx)
                {
                    pb_shape3D* shape = r->ceiling + ceiling_shape_idx;
                    auto ceilingDrawablePtr = new Drawable3D(texture, spec, shape->tris, shape->num_tris * 3, false);
                    ceilingDrawablePtr->pos.x = shape->pos.x;
                    ceilingDrawablePtr->pos.y = shape->pos.y;
                    ceilingDrawablePtr->pos.z = shape->pos.z;

                    std::shared_ptr<Drawable3D> ceilingDrawable(ceilingDrawablePtr);
                    addChild(ceilingDrawable);
                }
                free(r->ceiling);
            }

            free(r);
        }

        for (auto floor_wall_list_idx = 0; floor_wall_list_idx < f->num_wall_lists; ++floor_wall_list_idx)
        {
            pb_shape3D* wall_list = f->walls[floor_wall_list_idx];
            for (auto wall_shape_idx = 0; wall_shape_idx < f->wall_counts[floor_wall_list_idx]; ++wall_shape_idx)
            {
                pb_shape3D* wall = wall_list + wall_shape_idx;
                std::shared_ptr<Drawable3D> wallDrawable(new Drawable3D(exteriorWallTexture, spec,
                                                                        wall->tris, wall->num_tris * 3, false));
                wallDrawable->pos.x = wall->pos.x;
                wallDrawable->pos.y = wall->pos.y;
                wallDrawable->pos.z = wall->pos.z;

                addChild(wallDrawable);
            }
            ::free(wall_list);
        }
        ::free(f->walls);

        for (auto floor_door_idx = 0; floor_door_idx < f->num_doors; ++floor_door_idx)
        {
            pb_shape3D* door = f->doors + floor_door_idx;
            auto doorDrawablePtr = new Drawable3D((GLuint)0, spec, door->tris, door->num_tris * 3, false);
            doorDrawablePtr->setVisible(false);
            doorDrawablePtr->pos.x = door->pos.x;
            doorDrawablePtr->pos.y = door->pos.y;
            doorDrawablePtr->pos.z = door->pos.z;

            std::shared_ptr<Drawable3D> doorDrawable(doorDrawablePtr);
            addChild(doorDrawable);
        }
        free(f->doors);

        for (auto floor_window_idx = 0; floor_window_idx < f->num_windows; ++floor_window_idx)
        {
            pb_shape3D* window = f->windows + floor_window_idx;
            auto drawableWindowPtr = new Drawable3D((GLuint)0, spec, window->tris, window->num_tris * 3, false);
            drawableWindowPtr->setVisible(false);
            drawableWindowPtr->pos.x = window->pos.x;
            drawableWindowPtr->pos.y = window->pos.y;
            drawableWindowPtr->pos.x = window->pos.z;

            std::shared_ptr<Drawable3D> drawableWindow(drawableWindowPtr);
            addChild(drawableWindow);
        }
        free(f->windows);
        free(f);
    }

    free(floors);
}
