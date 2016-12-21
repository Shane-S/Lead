#pragma once

class Scene;

class Drawable {
    virtual void draw(Scene& scene) = 0;
};

