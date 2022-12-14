#pragma once

// ENGINE
#include "../ecs/system.h"
#include "../assetmgr/AssetManager.h"
#include "../game/map.h"

// EVENTS
#include "../events/KeyPressedEvent.h"

// COMPONENTS
#include "../components/CharacterController.h"
#include "../components/Transform.h"
#include "../components/Animation.h"
#include "../components/BoxCollider.h"
#include "../components/Camera.h"
#include "../components/RigidBody.h"
#include "../components/Health.h"
#include "../components/Sprite.h"
#include "../components/Projectile.h"
#include "../components/ProjectileEmitter.h"
#include "../components/Label.h"
#include "../components/MapTile.h"

// SYSTEMS
#include "../systems/CameraSystem.h"
#include "../systems/CollisionResolver.h"
#include "../systems/ProjectileEmitterSystem.h"
#include "../systems/Animator.h"
#include "../systems/CharacterSystem.h"
#include "../systems/DamageSystem.h"
#include "../systems/Movement.h"
#include "../systems/Render2D.h"
#include "../systems/RenderDebug.h"
#include "../systems/ProjectileSystem.h"
#include "../systems/RenderText2D.h"
