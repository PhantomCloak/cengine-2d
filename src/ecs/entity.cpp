#include "entity.h"
#include "world.h"

static World* world;
int Entity::GetId() const {
    return id;
}

void Entity::Kill() {
    world->KillEntity(*this);
}

void Entity::SetOwner(World* worldRef) {
    world = worldRef;
    owner = world->storage;
}

void Entity::Group(const std::string& group)
{
  world->GroupEntity(*this, group);
}

bool Entity::IsInGroup(const std::string& group) const
{
  return world->EntityIsInGroup(*this, group);
}

void Entity::Tag(const std::string& tag)
{
  world->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag) const
{
  return world->EntityHasTag(*this, tag);
}
