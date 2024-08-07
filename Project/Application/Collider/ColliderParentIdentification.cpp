#include "ColliderParentIdentification.h"

#include "../Object/Character/Player/Player.h"
#include "../Object/Obstacle/BaseObstacle.h"
#include "../Object/Character/Enemy/BaseEnemy.h"
#include "../Object/Weapon/BaseWeapon.h"
#include "../Object/Bonfire/Bonfire.h"
#include "../Object/Character/Player/TitlePlayer/TitlePlayer.h"
#include "../Object/RigidBodyObject/BaseRigidBodyObject.h"

MeshObject* ColliderParentIdentification::Execution(ColliderParentObject colliderPartner)
{

    MeshObject* result = nullptr;

    if (std::holds_alternative<Player*>(colliderPartner)) {
        result = std::get<Player*>(colliderPartner);
    }
    else if(std::holds_alternative<BaseObstacle*>(colliderPartner)){
        result = std::get<BaseObstacle*>(colliderPartner);
    }
    else if (std::holds_alternative<BaseEnemy*>(colliderPartner)) {
        result = std::get<BaseEnemy*>(colliderPartner);
    }
    else if (std::holds_alternative<BaseWeapon*>(colliderPartner)) {
        result = std::get<BaseWeapon*>(colliderPartner);
    }
    else if (std::holds_alternative<Bonfire*>(colliderPartner)) {
        result = std::get<Bonfire*>(colliderPartner);
    }
    else if (std::holds_alternative<TitlePlayer*>(colliderPartner)) {
        result = std::get<TitlePlayer*>(colliderPartner);
    }
    else if (std::holds_alternative<BaseRigidBodyObject*>(colliderPartner)) {
        result = std::get<BaseRigidBodyObject*>(colliderPartner);
    }

    return result;

}
