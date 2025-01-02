#pragma once

#include <cstdint>
#include "Position.h"
#include "Mass.h"
#include "EntityType.h"

class Entity
{
public:
    virtual ~Entity() = default;

    uint32_t getId() const { return id; }
    EntityType getEntityType() const { return entityType; }

    float getX() const { return position.coords.x; }
    float getY() const { return position.coords.y; }
    void setPosition(float x, float y)
    {
        position.coords.x = x;
        position.coords.y = y;
    }

    float getMass() const { return mass.getValue(); }
    void setMass(float m) { mass.setValue(m); }
    float getRadius() const { return mass.getRadius(); }

    virtual void update(float deltaTime, float xBoundary, float yBoundary) {}

protected:
    Entity(uint32_t _id, EntityType _type, float x, float y, float massValue)
        : id(_id), entityType(_type), position(x, y), mass(massValue)
    {
    }

    uint32_t id;
    EntityType entityType;

    Components::Position position;
    Components::Mass mass;
};
