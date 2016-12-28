#pragma once

class Scene;

class Drawable {
public:
    virtual void update(float dt) = 0;
    virtual void draw(Scene const& scene) = 0;

    void setVisible(bool visible) {visible_ = visible;}
    bool isVisible() {return visible_;}

protected:
    bool visible_;
};

