#include "wapper.h"

Wrapper::Wrapper(std::shared_ptr<World> world){
  this->world = world;
}

void Wrapper::AddCollider(int entityId, int size){
  Entity entity = world->GetEntity(entityId);
  entity.SetOwner(world.get());
  entity.AddComponent<BoxCollider>(size, size);
}

