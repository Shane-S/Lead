#include "Drawable3DContainer.h"

Drawable3DContainer::Drawable3DContainer()
: pos({ 0, 0, 0 }), scale({ 1, 1, 1 }), children_(), parent_(nullptr)
{

}

void Drawable3DContainer::addChild(std::shared_ptr<Drawable3DContainer> child)
{
    children_.push_back(child);
    child->parent_ = this;
}

void Drawable3DContainer::update(float dt)
{

}

void Drawable3DContainer::draw(Scene const& scene)
{
    for(auto& child : children_)
    {
        child->draw(scene);
    }
}