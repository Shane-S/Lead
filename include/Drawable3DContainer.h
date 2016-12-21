#pragma
#include <memory>
#include <glm/vec3.hpp>
#include <vector>
#include "Drawable.h"

class Drawable3DContainer : public Drawable {
public:
    Drawable3DContainer();
    Drawable3DContainer(Drawable3DContainer const& other) = delete;
    Drawable3DContainer(Drawable3DContainer&& other) = delete;
    Drawable3DContainer operator=(Drawable3DContainer& other) = delete;

    virtual void update(float dt) override;
    virtual void draw(Scene const& scene);

    virtual ~Drawable3DContainer() {};

    // This really should be a list of unique pointers, but I want to cast things...
    virtual void addChild(std::shared_ptr<Drawable3DContainer> child);

    // Transform info (I won't be rotating anything in the demo, so no need for orientation info)
    glm::vec3 pos;
    glm::vec3 scale;

private:
    Drawable3DContainer * parent_;
    std::vector<std::shared_ptr<Drawable3DContainer>> children_;
};